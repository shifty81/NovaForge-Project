# Project Manifest Schema

## Filename
`novaforge.project.json`

## Minimum Shape
```json
{
  "projectId": "novaforge",
  "displayName": "NovaForge",
  "projectType": "game",
  "manifestVersion": 1,
  "targets": {
    "client": "NovaForge.Client",
    "server": "NovaForge.Server",
    "tests": "NovaForge.Tests",
    "validation": "NovaForge.Validation"
  },
  "paths": {
    "sourceRoot": "Source",
    "contentRoot": "Content",
    "configRoot": "Config",
    "scriptsRoot": "Scripts",
    "testsRoot": "Tests",
    "packagingRoot": "Packaging",
    "releaseRoot": "Release"
  }
}
```
