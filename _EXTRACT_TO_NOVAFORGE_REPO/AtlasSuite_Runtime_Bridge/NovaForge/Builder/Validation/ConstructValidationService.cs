using System.Collections.Generic;

namespace Runtime.NovaForge.Builder.Validation;

public sealed class ConstructValidationService : IConstructValidationService
{
    public IReadOnlyList<ValidationResult> ValidateConstruct(string constructId)
    {
        return new List<ValidationResult>
        {
            new() { Category = "structure", Severity = ValidationSeverity.Pass, Message = $"{constructId}: structure pass (scaffold)." },
            new() { Category = "mount", Severity = ValidationSeverity.Pass, Message = $"{constructId}: mount pass (scaffold)." },
            new() { Category = "clearance", Severity = ValidationSeverity.Warn, Message = $"{constructId}: clearance check placeholder." }
        };
    }
}
