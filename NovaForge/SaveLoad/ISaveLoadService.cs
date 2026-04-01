namespace Runtime.NovaForge.SaveLoad
{
    public interface ISaveLoadService
    {
        void Save(string profileId);
        void Load(string profileId);
    }
}
