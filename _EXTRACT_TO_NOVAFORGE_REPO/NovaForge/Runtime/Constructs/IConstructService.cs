using System.Collections.Generic;

namespace Runtime.NovaForge.Constructs;

public interface IConstructService
{
    IReadOnlyList<ConstructRecord> GetActiveConstructs();
    ConstructRecord? FindById(string constructId);
}
