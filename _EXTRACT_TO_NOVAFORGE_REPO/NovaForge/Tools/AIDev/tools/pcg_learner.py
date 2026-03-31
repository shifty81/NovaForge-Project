"""
NovaForge Dev AI — PCG Learner (Phase 4)

Records user-approved object placements and style choices so the
procedural generation system can learn from them and suggest better
default positions in future rooms/scenes.
"""

import json
import logging
from pathlib import Path
from typing import Optional

log = logging.getLogger(__name__)


class PCGLearner:
    """
    Records approved placement metadata and queries it for PCG suggestions.

    Placement records are stored as JSON in:
        ai_dev/workspace/pcg_placements.json

    Each record captures:
        - object_type: what was placed (e.g. "CraftingConsole")
        - room_type: the context (e.g. "Engineering", "Quarters")
        - position_relative: normalised [0..1] within the room bounding box
        - wall_proximity: 0=center, 1=against wall
        - orientation_deg: 0/90/180/270
        - style_tags: list of style notes (e.g. ["sci-fi", "dark", "metallic"])
        - approved: True if user confirmed this placement
    """

    def __init__(self, repo_root: Path):
        self._db_path = repo_root / "ai_dev" / "workspace" / "pcg_placements.json"
        self._records: list[dict] = self._load()

    # ------------------------------------------------------------------
    # Recording
    # ------------------------------------------------------------------

    def record_placement(
        self,
        object_type: str,
        room_type: str,
        position_relative: tuple,
        wall_proximity: float = 0.5,
        orientation_deg: int = 0,
        style_tags: Optional[list] = None,
        approved: bool = True,
    ):
        """
        Store a placement decision after user approval.

        Args:
            object_type:        e.g. "CraftingConsole", "Bed", "DoorPanel"
            room_type:          e.g. "Engineering", "Quarters", "CommandBridge"
            position_relative:  (x, y, z) normalised to room bounding box [0..1]
            wall_proximity:     0.0 = room centre, 1.0 = against wall
            orientation_deg:    rotation in 90° increments
            style_tags:         optional style descriptors
            approved:           False if the user ultimately rejected this
        """
        record = {
            "object_type": object_type,
            "room_type": room_type,
            "position_relative": list(position_relative),
            "wall_proximity": round(wall_proximity, 3),
            "orientation_deg": orientation_deg,
            "style_tags": style_tags or [],
            "approved": approved,
        }
        self._records.append(record)
        self._save()
        log.info(
            f"[PCGLearner] Recorded {object_type} in {room_type} "
            f"pos={position_relative} approved={approved}"
        )

    def record_style(self, element_type: str, style_tags: list, approved: bool = True):
        """Record a GUI / shader style choice for an element type."""
        record = {
            "object_type": element_type,
            "room_type": "__style__",
            "position_relative": [0, 0, 0],
            "wall_proximity": 0.0,
            "orientation_deg": 0,
            "style_tags": style_tags,
            "approved": approved,
        }
        self._records.append(record)
        self._save()

    # ------------------------------------------------------------------
    # Querying
    # ------------------------------------------------------------------

    def suggest_placement(self, object_type: str, room_type: str) -> Optional[dict]:
        """
        Return the most common approved placement for this object/room pair.
        Returns None if no data is available.
        """
        matches = [
            r for r in self._records
            if r["object_type"] == object_type
            and r["room_type"] == room_type
            and r["approved"]
        ]
        if not matches:
            # Fall back to any room type
            matches = [r for r in self._records
                       if r["object_type"] == object_type and r["approved"]]
        if not matches:
            return None

        # Return centroid of positions
        xs = [r["position_relative"][0] for r in matches]
        ys = [r["position_relative"][1] for r in matches]
        zs = [r["position_relative"][2] for r in matches]
        avg_wall = sum(r["wall_proximity"] for r in matches) / len(matches)

        return {
            "position_relative": [
                round(sum(xs) / len(xs), 3),
                round(sum(ys) / len(ys), 3),
                round(sum(zs) / len(zs), 3),
            ],
            "wall_proximity": round(avg_wall, 3),
            "orientation_deg": matches[-1]["orientation_deg"],
            "based_on": len(matches),
        }

    def suggest_style(self, element_type: str) -> list:
        """Return the most common style tags for an element type."""
        matches = [
            r for r in self._records
            if r["object_type"] == element_type
            and r["room_type"] == "__style__"
            and r["approved"]
        ]
        if not matches:
            return []
        # Collect and rank tags by frequency
        tag_counts: dict[str, int] = {}
        for r in matches:
            for tag in r.get("style_tags", []):
                tag_counts[tag] = tag_counts.get(tag, 0) + 1
        return sorted(tag_counts, key=lambda t: -tag_counts[t])

    def stats(self) -> dict:
        total = len(self._records)
        approved = sum(1 for r in self._records if r["approved"])
        types = len({r["object_type"] for r in self._records})
        return {"total": total, "approved": approved, "unique_types": types}

    # ------------------------------------------------------------------
    # Persistence
    # ------------------------------------------------------------------

    def _load(self) -> list:
        if self._db_path.exists():
            try:
                return json.loads(self._db_path.read_text(encoding="utf-8"))
            except (OSError, json.JSONDecodeError):
                pass
        return []

    def _save(self):
        try:
            self._db_path.parent.mkdir(parents=True, exist_ok=True)
            self._db_path.write_text(
                json.dumps(self._records, indent=2), encoding="utf-8"
            )
        except OSError as e:
            log.warning(f"Could not save PCG placement data: {e}")
