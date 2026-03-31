"""
Installer Builder — stubs for creating Windows installers and portable packages.

Supports NSIS, portable zip, and MSI installer creation stubs for the
SwissAgent offline release pipeline.
"""

from pathlib import Path
from tools.zip_tools import ZipTools


class InstallerBuilder:
    """Stubs for creating Windows installers and portable packages."""

    def __init__(self):
        self._zip = ZipTools()

    # ------------------------------------------------------------------
    def create_nsis_installer(self, app_name, version, source_dir, output_path):
        """Generate an NSIS installer script and invoke makensis (stub)."""
        script = self.generate_nsis_script(app_name, version, source_dir,
                                           f"C:\\Program Files\\{app_name}")
        nsi_path = Path(output_path).with_suffix(".nsi")
        nsi_path.parent.mkdir(parents=True, exist_ok=True)
        nsi_path.write_text(script, encoding="utf-8")
        print(f"[InstallerBuilder] NSIS script → {nsi_path} (invoke makensis manually)")
        return str(nsi_path)

    def create_portable_zip(self, source_dir, app_name, version, output_dir):
        """Create a portable zip release package."""
        out = self._zip.create_release_zip(source_dir, version, output_dir)
        print(f"[InstallerBuilder] portable zip → {out}")
        return out

    def create_msi(self, app_name, version, source_dir, output_path):
        """Create an MSI installer via WiX (stub — requires WiX toolset)."""
        print(f"[InstallerBuilder] MSI {app_name} v{version} → {output_path} (stub)")
        return output_path

    def sign_exe(self, exe_path, cert_path, cert_password=""):
        """Sign an executable using signtool.exe (stub)."""
        print(f"[InstallerBuilder] sign {exe_path} with {cert_path} (stub)")
        return exe_path

    def generate_nsis_script(self, app_name, version, source_dir, install_dir):
        """Return a minimal NSIS installer script as a string."""
        return (
            f'; NSIS installer script for {app_name} v{version}\n'
            f'!define APPNAME "{app_name}"\n'
            f'!define VERSION "{version}"\n'
            f'!define INSTALLDIR "{install_dir}"\n'
            f'\n'
            f'Name "${{APPNAME}} ${{VERSION}}"\n'
            f'OutFile "{app_name}-{version}-setup.exe"\n'
            f'InstallDir "${{INSTALLDIR}}"\n'
            f'\n'
            f'Section "MainSection" SEC01\n'
            f'  SetOutPath "$INSTDIR"\n'
            f'  File /r "{source_dir}\\*.*"\n'
            f'SectionEnd\n'
        )
