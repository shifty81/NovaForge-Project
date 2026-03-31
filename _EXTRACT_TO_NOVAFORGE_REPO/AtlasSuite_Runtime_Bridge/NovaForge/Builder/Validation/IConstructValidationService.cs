using System.Collections.Generic;

namespace Runtime.NovaForge.Builder.Validation;

public interface IConstructValidationService
{
    IReadOnlyList<ValidationResult> ValidateConstruct(string constructId);
}
