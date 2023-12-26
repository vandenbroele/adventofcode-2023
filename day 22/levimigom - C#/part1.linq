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

	pile.FinaliseStack();
	pile.CountUselessBricks().Dump();
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
}

internal sealed class Pile{
	private const int MaximumX = 10;
	private const int MaximumY = 10;
	private const int MaximumZ = 512; 
	
	private readonly List<Brick> _bricks;
	private readonly Brick[,,] _stack;

	public Pile(){
		_bricks = new List<Brick>();
		_stack = new Brick[MaximumX, MaximumY, MaximumZ];
	}

	private void AddBrickToStack(Brick brick){
		for (int x = brick.x1; x <= brick.x2; x++) {
			for (int y = brick.y1; y <= brick.y2; y++) {
				for (int z = brick.z1; z <= brick.z2; z++) {
					_stack[x, y, z] = brick;
				}
			}
		}
	}

	private void RemoveBrickFromStack(Brick brick){
		for (int x = brick.x1; x <= brick.x2; x++) {
			for (int y = brick.y1; y <= brick.y2; y++) {
				for (int z = brick.z1; z <= brick.z2; z++) {
					_stack[x, y, z] = null;
				}
			}
		}
	}

	public void AddBrick(Brick brick){
		_bricks.Add(brick);
		AddBrickToStack(brick);
	}
	
	public void FinaliseStack(){
		Brick brick = _bricks.FirstOrDefault(b => BrickCanGoDown(b));

		while (brick != null) {
			MoveBrickDown(brick);
			while(BrickCanGoDown(brick)){
				MoveBrickDown(brick);
			}

			brick = _bricks.FirstOrDefault(b => BrickCanGoDown(b));
		}
	}
	
	public int CountUselessBricks(){
		int count = 0;

		foreach (var brick in _bricks) {
			RemoveBrickFromStack(brick);
			
			if(_bricks.All(b => !BrickCanGoDown(b))){
				count++;
			}

			AddBrickToStack(brick);
		}
		
		return count;
	}

	private bool BrickCanGoDown(Brick brick){
		for (int x = brick.x1; x <= brick.x2; x++) {
			for (int y = brick.y1; y <= brick.y2; y++) {
				for (int z = brick.z1; z <= brick.z2; z++) {
					if(z == 1 || (_stack[x, y, z - 1] != null && !System.Object.ReferenceEquals(brick, _stack[x, y, z - 1]))){
						return false;
					}
				}
			}
		}

		return true;
	}

	private void MoveBrickDown(Brick brick){
		RemoveBrickFromStack(brick);

		brick.z1--;
		brick.z2--;

		AddBrickToStack(brick);
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
	
	pile.FinaliseStack();
	Assert.Equal(5, pile.CountUselessBricks());
}
#endregion