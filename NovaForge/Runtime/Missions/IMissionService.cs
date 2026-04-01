namespace Runtime.NovaForge.Missions
{
    public interface IMissionService
    {
        void Prime(string missionId);
        void Offer(string missionId);
    }
}
