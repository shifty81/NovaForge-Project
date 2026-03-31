# Local Commands

## PowerShell
```powershell
pwsh -ExecutionPolicy Bypass -File .\Scripts\bootstrap_repo.ps1 -RemoteUrl "<your repo url>"
pwsh -ExecutionPolicy Bypass -File .\Scripts\validate_project.ps1
git branch -M main
git push -u origin main
```

## Bash
```bash
./Scripts/bootstrap_repo.sh "NovaForge" "<your repo url>"
pwsh -ExecutionPolicy Bypass -File ./Scripts/validate_project.ps1
git branch -M main
git push -u origin main
```
