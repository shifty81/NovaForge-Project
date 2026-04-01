using AtlasSuite.Core.Abstractions;
using AtlasSuite.Core.Commands;
using AtlasSuite.Core.Docking;
using AtlasSuite.Plugins.Abstractions;

namespace AtlasSuite.Plugin.Sample;

public sealed class SampleSalvagePlugin : IAtlasSuitePlugin
{
    public string Id => "plugin.sample.salvage";
    public string DisplayName => "Sample Salvage Tools";
    public string Version => "1.0.0";
    public string Description => "Adds a salvage debug panel and playtest command for NovaForge salvage scenarios.";
    public AtlasPluginKind Kind => AtlasPluginKind.ProjectExtension;

    public void Register(IPanelRegistry panelRegistry, ICommandBus commandBus)
    {
        panelRegistry.Register(new PanelDescriptor(
            Id: "panel.salvageDebug",
            Title: "Salvage Debug",
            DefaultDock: DockZone.Right,
            ViewKey: "SalvageDebugPanel",
            Category: "Gameplay"));

        commandBus.Register(new CommandDefinition(
            Id: "playtest.salvage",
            Label: "Run Salvage Playtest",
            Category: "Playtest",
            Handler: _ => Task.CompletedTask,
            Description: "Starts the salvage scenario stub."));
    }

    public Task InitializeAsync(CancellationToken cancellationToken = default)
        => Task.CompletedTask;

    public Task ShutdownAsync(CancellationToken cancellationToken = default)
        => Task.CompletedTask;
}
