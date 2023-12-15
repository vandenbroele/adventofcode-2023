Console.WriteLine((
    from line in File.ReadAllLines("input.txt")
    let number = new string((
        from c in line
        where c is >= '0' and <= '9'
        select c).ToArray())
    select int.Parse($"{number[0]}{number[^1]}")).Sum());