using Runtime.NovaForge.SaveLoad;

namespace Runtime.NovaForge.DevWorld.Terminals
{
    public sealed class SaveCheckpointTerminal : IDevTerminal
    {
        private readonly ISaveLoadService _saveLoadService;

        public SaveCheckpointTerminal(ISaveLoadService saveLoadService)
        {
            _saveLoadService = saveLoadService;
        }

        public string TerminalId => "checkpoint_a";
        public string DisplayName => "Save / Load Checkpoint";

        public void Interact()
        {
            _saveLoadService.Save("dev_local");
        }

        public void Load()
        {
            _saveLoadService.Load("dev_local");
        }
    }
}
