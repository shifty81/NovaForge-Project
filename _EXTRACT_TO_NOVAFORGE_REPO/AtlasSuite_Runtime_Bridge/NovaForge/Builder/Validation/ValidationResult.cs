namespace Runtime.NovaForge.Builder.Validation;

public sealed class ValidationResult
{
    public string Category { get; set; } = string.Empty;
    public ValidationSeverity Severity { get; set; } = ValidationSeverity.Pass;
    public string Message { get; set; } = string.Empty;
}
