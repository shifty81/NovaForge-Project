"""
NovaForge Dev AI — Stable Diffusion Interface

Stub integration layer for local Stable Diffusion / AUTOMATIC1111 WebUI.
All generation methods are offline-safe stubs; `is_available()` performs
a real connectivity check and returns False gracefully when SD is offline.
"""

import logging
import os
from pathlib import Path
from typing import Optional

log = logging.getLogger(__name__)


class StableDiffusionInterface:
    """
    Provides a stub interface to a locally-running Stable Diffusion WebUI
    (AUTOMATIC1111 compatible REST API at http://127.0.0.1:7860 by default).

    When SD is unavailable the methods still return valid placeholder paths
    so the agent loop can continue without interruption.
    """

    def __init__(
        self,
        api_url: str = "http://127.0.0.1:7860",
        model: str = "v1-5-pruned",
        output_dir: str = "ai_dev/workspace/sd_output",
    ):
        """
        Args:
            api_url:    Base URL of the SD WebUI REST API.
            model:      Model checkpoint name to load.
            output_dir: Directory where generated images are stored.
        """
        self.api_url = api_url
        self.model = model
        self.output_dir = Path(output_dir)
        self.output_dir.mkdir(parents=True, exist_ok=True)

    def is_available(self) -> bool:
        """
        Check whether the Stable Diffusion WebUI is reachable.

        Attempts a lightweight HTTP GET against the API. Returns False
        gracefully on any network error so offline workflows are unaffected.

        Returns:
            True if the WebUI responded with HTTP 200, False otherwise.
        """
        try:
            import urllib.request
            with urllib.request.urlopen(f"{self.api_url}/sdapi/v1/samplers", timeout=2) as resp:
                return resp.status == 200
        except Exception:
            log.debug("[StableDiffusionInterface] WebUI not reachable at %s", self.api_url)
            return False

    def txt2img(
        self,
        prompt: str,
        negative_prompt: str = "",
        steps: int = 20,
        width: int = 512,
        height: int = 512,
        seed: int = -1,
    ) -> str:
        """
        Text-to-image generation (stub).

        Args:
            prompt:          Positive prompt for image generation.
            negative_prompt: Negative prompt to guide the model away from.
            steps:           Number of diffusion sampling steps.
            width:           Output image width in pixels.
            height:          Output image height in pixels.
            seed:            RNG seed (-1 for random).

        Returns:
            Placeholder path to the generated image PNG.
        """
        out_path = str(self.output_dir / f"txt2img_{abs(seed)}.png")
        print(f"[StableDiffusionInterface] txt2img: prompt={prompt!r}, steps={steps}, "
              f"size={width}x{height}, seed={seed} → {out_path} (stub)")
        log.info("[StableDiffusionInterface] txt2img stub: %s", out_path)
        return out_path

    def img2img(
        self,
        prompt: str,
        input_path: str,
        denoising_strength: float = 0.75,
    ) -> str:
        """
        Image-to-image generation (stub).

        Args:
            prompt:             Prompt describing the desired output.
            input_path:         Path to the source image.
            denoising_strength: How much to deviate from the source (0–1).

        Returns:
            Placeholder path to the generated image PNG.
        """
        input_stem = Path(input_path).stem
        out_path = str(self.output_dir / f"img2img_{input_stem}_out.png")
        print(f"[StableDiffusionInterface] img2img: input={input_path!r}, "
              f"strength={denoising_strength} → {out_path} (stub)")
        log.info("[StableDiffusionInterface] img2img stub: %s", out_path)
        return out_path

    def inpaint(
        self,
        prompt: str,
        input_path: str,
        mask_path: str,
    ) -> str:
        """
        Inpainting generation (stub).

        Args:
            prompt:     Prompt describing what to paint into the masked area.
            input_path: Path to the base image.
            mask_path:  Path to the mask image (white = inpaint area).

        Returns:
            Placeholder path to the inpainted output image PNG.
        """
        input_stem = Path(input_path).stem
        out_path = str(self.output_dir / f"inpaint_{input_stem}_out.png")
        print(f"[StableDiffusionInterface] inpaint: input={input_path!r}, "
              f"mask={mask_path!r} → {out_path} (stub)")
        log.info("[StableDiffusionInterface] inpaint stub: %s", out_path)
        return out_path

    def controlnet(
        self,
        prompt: str,
        input_path: str,
        control_type: str = "canny",
    ) -> str:
        """
        ControlNet-guided generation (stub).

        Args:
            prompt:       Prompt for the controlled generation.
            input_path:   Path to the control input image.
            control_type: ControlNet type (canny, depth, pose, …).

        Returns:
            Placeholder path to the generated image PNG.
        """
        input_stem = Path(input_path).stem
        out_path = str(self.output_dir / f"controlnet_{control_type}_{input_stem}.png")
        print(f"[StableDiffusionInterface] controlnet: type={control_type!r}, "
              f"input={input_path!r} → {out_path} (stub)")
        log.info("[StableDiffusionInterface] controlnet stub: %s", out_path)
        return out_path
