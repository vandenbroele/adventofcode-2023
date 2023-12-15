Console.WriteLine((
    from line in File.ReadAllLines("input.txt")
    let number = new string((
        from c in line
        where c is >= '0' and <= '9'
        select c).ToArray())
    select int.Parse($"{number[0]}{number[^1]}")).Sum());

Console.WriteLine((
    from line in File.ReadAllLines("input.txt")
    let number = new string(ExtractDigits(line).ToArray())
    select int.Parse($"{number[0]}{number[^1]}")).Sum());

IEnumerable<char> ExtractDigits(string line)
{
    (string word, char digit)[] words =
    {
        ("one", '1'), ("two", '2'), ("three", '3'),
        ("four", '4'), ("five", '5'), ("six", '6'),
        ("seven", '7'), ("eight", '8'), ("nine", '9')
    };

    for (var i = 0; i < line.Length; i++)
    {
        if (line[i] is >= '1' and <= '9')
        {
            yield return line[i];
        }
        else
        {
            var match = (
                from word in words
                where i + word.word.Length <= line.Length
                where line[i..(i + word.word.Length)] == word.word
                select word).SingleOrDefault();
            if (match != default)
            {
                yield return match.digit;
            }
        }
    }
}