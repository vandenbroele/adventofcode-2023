<Query Kind="Program">
  <Reference Relative="input.txt">C:\Development\adventofcode-2023\day 20\levimigom - C#\input.txt</Reference>
  <Namespace>Xunit</Namespace>
</Query>

#load "xunit"

void Main()
{
	ModuleManager manager = new ModuleManager();
	
	foreach(var line in File.ReadAllLines("input.txt")){
		manager.AddModule(line);
	}
	
	for(int i = 0; i < 1000; i++){
		manager.PressButton();
	}
	
	manager.LowPulseCount.Dump();
	manager.HighPulseCount.Dump();
	(manager.LowPulseCount * manager.HighPulseCount).Dump();
}

internal enum Pulse {
	High,
	Low
};

internal interface IModule {
	public void HandlePulse(Pulse pulse, string source, Queue<(Pulse pulse, string source, string destination)> pulses, List<string> sources);
}

internal abstract class ModuleBase {
	protected string _name;
	protected List<string> _destinations;
	
	public ModuleBase(string name, List<string> destinations)
	{
		_name = name;
		_destinations = destinations;
	}

	protected void SendPulse(Pulse pulse, Queue<(Pulse pulse, string source, string destination)> pulses){
		foreach(var destination in _destinations){
			pulses.Enqueue((pulse, _name, destination));
		}
	}
}

internal class FlipFlopModule : ModuleBase, IModule {
	private bool _state;
	
	public FlipFlopModule(string name, List<string> destinations) : base(name, destinations)
	{
		_state = false;
	}
	
	public void HandlePulse(Pulse pulse, string source, Queue<(Pulse pulse, string source, string destination)> pulses, List<string> sources){
		if(pulse == Pulse.High){
			return;
		}
		
		if(_state){
			SendPulse(Pulse.Low, pulses);
		} else {
			SendPulse(Pulse.High, pulses);
		}
		
		_state = !_state;
	}
}

internal class ConjunctionModule : ModuleBase, IModule {
	private Dictionary<string, Pulse> _previousPulse;
	
	public ConjunctionModule(string name, List<string> destinations) : base(name, destinations)
	{
		_previousPulse = new Dictionary<string, Pulse>();
	}

	public void HandlePulse(Pulse pulse, string source, Queue<(Pulse pulse, string source, string destination)> pulses, List<string> sources)
	{
		_previousPulse[source] = pulse;
		
		if(sources.All(s => _previousPulse.ContainsKey(s) && _previousPulse[s] == Pulse.High)){
			SendPulse(Pulse.Low, pulses);
		} else {
			SendPulse(Pulse.High, pulses);
		}
	}
}

internal class BroadcastModule : ModuleBase, IModule {
	public BroadcastModule(string name, List<string> destinations) : base(name, destinations)
	{
		
	}

	public void HandlePulse(Pulse pulse, string source, Queue<(Pulse pulse, string source, string destination)> pulses, List<string> sources)
	{
		SendPulse(pulse, pulses);
	}
}

internal class ModuleManager {
	private Dictionary<string, IModule> _modules;
	private Dictionary<string, List<string>> _sources;
	
	public int LowPulseCount { get; private set; }
	public int HighPulseCount { get; private set; }

	public ModuleManager()
	{
		_modules = new Dictionary<string, IModule>();
		_sources = new Dictionary<string, List<string>>();

		LowPulseCount = 0;
		HighPulseCount = 0;
	}
	
	public void AddModule(string description){
		var parts = description.Split(" -> ");
		
		string name = parts[0];
		List<string> destinations = parts[1].Split(", ").ToList();

		if (name == "broadcaster") {
			_modules[name] = new BroadcastModule(name, destinations);
			AddSources(name, destinations);
		}
		else if (name.StartsWith("%")) {
			_modules[name[1..]] = new FlipFlopModule(name[1..], destinations);
			AddSources(name[1..], destinations);
		}
		else {
			_modules[name[1..]] = new ConjunctionModule(name[1..], destinations);
			AddSources(name[1..], destinations);
		}
	}
	
	private void AddSources(string source, List<string> destinations){
		if(!_sources.ContainsKey(source)){
			_sources[source] = new List<string>();
		}
	
		foreach(var destination in destinations){
			if(!_sources.ContainsKey(destination)){
				_sources[destination] = new List<string>();
			}
			_sources[destination].Add(source);
		}
	}

	public void PressButton(){
		Queue<(Pulse pulse, string source, string destination)> queue = new Queue<(Pulse, string, string)>();
		
		queue.Enqueue((Pulse.Low, "button", "broadcaster"));
		
		while(0 < queue.Count()){
			(Pulse pulse, string source, string destination) = queue.Dequeue();

			//Console.WriteLine($"{source} -{pulse.ToString()}-> {destination}");
			
			if(pulse == Pulse.Low){
				LowPulseCount++;
			} else {
				HighPulseCount++;
			}
			
			if(_modules.ContainsKey(destination)){
				_modules[destination].HandlePulse(pulse, source, queue, _sources[destination]);
			}
		}
	}
}

#region private::Tests
[Fact]
public void TestExample(){
	ModuleManager manager = new ModuleManager();
	
	manager.AddModule("broadcaster -> a, b, c");
	manager.AddModule("%a -> b");
	manager.AddModule("%b -> c");
	manager.AddModule("%c -> inv");
	manager.AddModule("&inv -> a");
	
	for(int i = 0; i < 1000; i++){
		manager.PressButton();
	}

	Assert.Equal(32000000, manager.LowPulseCount * manager.HighPulseCount);
}
#endregion