using System.Collections.Generic;
using Runtime.NovaForge.DevWorld.Terminals;

namespace Runtime.NovaForge.DevWorld
{
    public sealed class DevWorldRegistry : IDevWorldRegistry
    {
        private readonly List<IDevWorldResettable> _registered = new();

        public void Register(IDevWorldResettable resettable)
        {
            _registered.Add(resettable);
        }

        public void RegisterDefaults()
        {
            // Hook runtime-discovered entities here as systems come online.
        }

        public void ResetAll()
        {
            foreach (var item in _registered)
            {
                item.ResetToDefaultState();
            }
        }
    }

    public interface IDevWorldResettable
    {
        void ResetToDefaultState();
    }
}
