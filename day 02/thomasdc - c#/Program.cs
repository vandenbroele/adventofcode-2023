using System.Text.RegularExpressions;

var sum = 0;
var power = 0;
foreach (var line in File.ReadAllLines("input.txt"))
{
    var match = Regex.Match(line, "Game (?<game_id>\\d+):(?<reveals>.*)");
    var gameId = int.Parse(match.Groups["game_id"].Value);
    var reveals = new Dictionary<string, int>();
    foreach (var reveal in match.Groups["reveals"].Value.Split(';'))
    {
        foreach (var colorMatch in Regex.Matches(reveal, "(?<count>\\d+) (?<color>\\w+)").Cast<Match>())
        {
            var color = colorMatch.Groups["color"].Value;
            var count = int.Parse(colorMatch.Groups["count"].Value);
            reveals[color] = Math.Max(reveals.GetValueOrDefault(color, 0), count);
        }
    }

    if (IsPossible(reveals, new[] { (12, "red"), (13, "green"), (14, "blue") }))
    {
        sum += gameId;
    }

    power += reveals.Values.Aggregate(1, (x, y) => x * y);
}

Console.WriteLine((sum, power));

bool IsPossible(IReadOnlyDictionary<string, int> reveals, (int count, string color)[] cubes)
{
    return cubes.All(cube => reveals.GetValueOrDefault(cube.color, 0) <= cube.count);
}
