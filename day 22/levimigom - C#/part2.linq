<Query Kind="Program">
  <Reference Relative="input.txt">C:\Development\adventofcode-2023\day 22\input.txt</Reference>
  <Namespace>Xunit</Namespace>
</Query>

#load "xunit"

void Main()
{
	Pile pile = new Pile();
	
	foreach(var description in File.ReadAllLines("input.txt")){
		pile.AddBrick(ParseBrick(description));
	}

	pile.SumFallingBricks().Dump();
}

Brick ParseBrick(string description){
	int[] parts = description.Replace('~', ',').Split(',').Select(int.Parse).ToArray();

	Brick brick = new Brick(){
		x1 = parts[0],
		y1 = parts[1],
		z1 = parts[2],
		x2 = parts[3],
		y2 = parts[4],
		z2 = parts[5]
	};
	
	return brick;
}

internal sealed class Brick {
	public int x1 { get; set; }
	public int y1 { get; set; }
	public int z1 { get; set; }
	public int x2 { get; set; }
	public int y2 { get; set; }
	public int z2 { get; set; }
	
	public Brick()
	{
		
	}
	
	public Brick(Brick brick){
		this.x1 = brick.x1;
		this.y1 = brick.y1;
		this.z1 = brick.z1;
		this.x2 = brick.x2;
		this.y2 = brick.y2;
		this.z2 = brick.z2;
	}
}

internal sealed class Stack
{
	private const int MaximumX = 10;
	private const int MaximumY = 10;
	private const int MaximumZ = 512;
	
	private readonly HashSet<Brick> _bricks;
	private readonly Brick[,,] _stack;

	public Stack()
	{
		_bricks = new HashSet<Brick>();
		_stack = new Brick[MaximumX, MaximumY, MaximumZ];
	}

	public void AddBrickToStack(Brick brick)
	{
		_bricks.Add(brick);
		for (int x = brick.x1; x <= brick.x2; x++) {
			for (int y = brick.y1; y <= brick.y2; y++) {
				for (int z = brick.z1; z <= brick.z2; z++) {
					_stack[x, y, z] = brick;
				}
			}
		}
	}

	public void RemoveBrickFromStack(Brick brick)
	{
		_bricks.Remove(brick);
		for (int x = brick.x1; x <= brick.x2; x++) {
			for (int y = brick.y1; y <= brick.y2; y++) {
				for (int z = brick.z1; z <= brick.z2; z++) {
					_stack[x, y, z] = null;
				}
			}
		}
	}

	public int Finalise()
	{
		HashSet<Brick> movedBricks = new HashSet<Brick>();
		Brick brick = _bricks.FirstOrDefault(b => BrickCanGoDown(b));

		while (brick != null) {
			movedBricks.Add(brick);
			MoveBrickDown(brick);
			while (BrickCanGoDown(brick)) {
				MoveBrickDown(brick);
			}

			brick = _bricks.FirstOrDefault(b => BrickCanGoDown(b));
		}
		
		return movedBricks.Count();
	}

	public bool BrickCanGoDown(Brick brick){
		for (int x = brick.x1; x <= brick.x2; x++) {
			for (int y = brick.y1; y <= brick.y2; y++) {
				for (int z = brick.z1; z <= brick.z2; z++) {
					if (z == 1 || (_stack[x, y, z - 1] != null && !System.Object.ReferenceEquals(brick, _stack[x, y, z - 1]))) {
						return false;
					}
				}
			}
		}

		return true;
	}

	private void MoveBrickDown(Brick brick)
	{
		RemoveBrickFromStack(brick);

		brick.z1--;
		brick.z2--;

		AddBrickToStack(brick);
	}
}

internal sealed class Pile {
	private readonly List<Brick> _bricks;
	private readonly Stack _stack;
	
	public Pile(){
		_bricks = new List<Brick>();
		_stack = new Stack();	
	}

	public void AddBrick(Brick brick){
		_bricks.Add(brick);
		_stack.AddBrickToStack(brick);
	}
	
	public int SumFallingBricks(){
		_stack.Finalise();
		int sum = 0;
		
		foreach(var brick in _bricks){
			sum += CountBricksAffectedByRemoval(brick);
		}
		
		return sum;
	}
	
	private int CountBricksAffectedByRemoval(Brick brick){
		Stack stack = new Stack();
		
		foreach(var b in _bricks){
			if(!System.Object.ReferenceEquals(b, brick)){
				stack.AddBrickToStack(new Brick(b));
			}
		}
		
		return stack.Finalise();
	}
}

#region private::Tests
[Fact]
public void TestParseBrick(){
	Brick brick = ParseBrick("1,0,1~1,2,1");
	Assert.Equal(1, brick.x1);
	Assert.Equal(0, brick.y1);
	Assert.Equal(1, brick.z1);
	Assert.Equal(1, brick.x2);
	Assert.Equal(2, brick.y2);
	Assert.Equal(1, brick.z2);
}

[Fact]
public void TestPile(){
	string[] descriptions = {
		"1,0,1~1,2,1",
		"0,0,2~2,0,2",
		"0,2,3~2,2,3",
		"0,0,4~0,2,4",
		"2,0,5~2,2,5",
		"0,1,6~2,1,6",
		"1,1,8~1,1,9"
	};
	
	Pile pile = new Pile();
	
	foreach(var description in descriptions){
		pile.AddBrick(ParseBrick(description));
	}
	
	Assert.Equal(7, pile.SumFallingBricks());
}
#endregion