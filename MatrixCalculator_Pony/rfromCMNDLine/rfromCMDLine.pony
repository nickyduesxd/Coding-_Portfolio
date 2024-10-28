//NOTES: It works, problem is with printing the result, might need to do some promises shit
// NEXT: Add other functionality described in the problem
//these include add, sub, scalmul, mul, transpose and determinant

// CURRENT STATUS: 
// Transpose works :)
// determinant needs to be added - shit gets complicated fast - worry about later
// Adjoint needs to be added as well
// Add, sub, scalmul, and mul work functionally
// TODO Need to fix node class to print properly (dealock with environment)


use "files"
use "time"
// nodes based on row numbers
actor Node
    var _env: Env
    var _row: U32
    var _col: U32
    var _m1: Array[U32] iso
    var _m2: Array[U32] iso
    var _m3: Array[U32] = []
    var _operation: String
    var _nodeNum: U32
    new create(env: Env, row: U32, col: U32,  m1: Array[U32] iso, m2: Array[U32] iso, operation: String, nodeNum: U32) =>
        _env = env
        _row = row
        _col = col
        _m1= consume m1
        _m2 = consume m2
        _operation = operation 
        _nodeNum = nodeNum
        if _operation == "add" then
            add()
        elseif _operation == "sub" then
            sub()
        elseif _operation == "mul" then
            mul()
        elseif _operation == "scalmul" then
            mul()
        else
            _env.out.print("improper operator")
        end

        print()
  
    fun ref add() =>
        var count: U32 = 0
        var size: U32 = _row * _col
        while count < size do
            var value: U32 = 0
            try value = _m1.apply(count.usize())? + _m2.apply(count.usize())? end
            _m3.push(value)
            count = count + 1
        end
    fun ref sub() =>
        var count: U32 = 0
        var size: U32 = _row * _col
        while count < size do
            var value: U32 = 0
            try value = _m1.apply(count.usize())? - _m2.apply(count.usize())? end
            _m3.push(value)
            count = count + 1
        end
    fun ref mul() =>
        var count: U32 = 0
        var size: U32 = _row * _col
        while count < size do
            var value: U32 = 0
            try value = _m1.apply(count.usize())? * _m2.apply(count.usize())? end
            _m3.push(value)
            count = count + 1
        end
    fun ref print() =>
        var count: U32 = 0
        var size: U32 = _row * _col
        while count < size do
            if (count %% _col) == 0 then  
                _env.out.print("")
            else
               try  _env.out.write(_m3.apply(count.usize())?.string() + " ") end
            end
            count = count + 1

        end
        //_env.out.print("")

    // https://github.com/ponylang/ponyc/blob/main/examples/mixed/main.pony

actor Main
	new create(env: Env) =>
    
    //TODO
    //add in some instructions so the user knows to make the files with a specific format and knows the possible inputs like "add matrix matrix"

    var operation: String = ""
    var col: U32 = 0
    var row: U32 = 0
    var m1: Array[U32] = []
    var m2: Array[U32] = []

    try
    operation = env.args(1)? end

    var file_name : String = ""

    if( (((operation == "transp") /*LEAVE THIS OUT UNLESS WE HAVE TIME- or (operation == "det")*/)) and (env.args.size().u32() == 3)) then
        try file_name = env.args.slice(2).apply(0)?.string() end
        let path = FilePath(FileAuth(env.root), file_name)
        match OpenFile(path)
        | let file: File =>
            let allin: String = file.read_string(1024)
            let delimiter: String = " \n"
            let split_array: Array[String] = allin.split(delimiter)
            try 
            col = split_array.apply(0)?.u32()?
            row = split_array.apply(1)?.u32()?
            var count: U32 = 2
            while count <= (split_array.size().u32()-2) do
                m1.push(split_array.apply(count.usize())?.u32()?)
                count = count + 1
            end
            end
            if(operation == "transp") then
                transpose(env, m1, row, col)
            /* LEAVE THIS OUT UNLESS WE HAVE TIME
            elseif((operation == "det") and (col == row)) then
                determinant(env, m1, row, col)
            elseif(operation == "adj") then
                env.out.print("In the works")
                // adj()
            */
            else
                env.out.print("Rejected: Must be a square matrix.")
            end
        else
            env.err.print("Error opening file '" + file_name + "'")
        end
    elseif((((operation == "add") or (operation == "sub")) or (operation == "mul") or (operation == "scalmul")) and (env.args.size().u32() == 4)) then
        try file_name = env.args.slice(2).apply(0)?.string() end
        let path = FilePath(FileAuth(env.root), file_name)
        match OpenFile(path)
        | let file: File =>
            let allin: String = file.read_string(1024)
            let delimiter: String = " \n"
            let split_array: Array[String] = allin.split(delimiter)
            try 
            col = split_array.apply(0)?.u32()?
            row = split_array.apply(1)?.u32()?
            var count: U32 = 2
            while count <= (split_array.size().u32()-2) do
                m1.push(split_array.apply(count.usize())?.u32()?)
                count = count + 1
            end
            end
        else
            env.err.print("Error opening file '" + file_name + "'")
        end

        if (operation == "scalmul") then
            var count: U32 = 0
            var scal: U32 = 0
            try scal = env.args(3)?.u32()? end
            while count < (col * row) do
                m2.push(scal)
                count = count + 1
            end
        else
            file_name= ""
            try file_name = env.args.slice(2).apply(1)?.string() end
            let newpath = FilePath(FileAuth(env.root), file_name)
            match OpenFile(newpath)
            | let file: File =>
                let allin: String = file.read_string(1024)
                let delimiter: String = " \n"
                let split_array: Array[String] = allin.split(delimiter)
                try 
                var count: U32 = 2
                while count <= (split_array.size().u32()-2) do
                    m2.push(split_array.apply(count.usize())?.u32()?)
                    count = count + 1
                end
            end            
        else
            env.err.print("Error opening file '" + file_name + "'")
        end
        end
        env.out.print(m1.size().string())
    env.out.print(m2.size().string())
    
    var newrow1: U32 = row/3 
    var newrow2: U32 = row/3
    var newrow3: U32 = row/3

    if (newrow1*3) < row then
        newrow1 = newrow1 + 1
    end

    if (newrow1 + (newrow2*2)) < row then
        newrow2 = newrow2 + 1
    end

//https://patterns.ponylang.io/data-sharing/copying#solution
    

    let n1m1: Array[U32] iso = recover Array[U32] end
    let n1m2: Array[U32] iso = recover Array[U32] end
    let n2m1: Array[U32] iso = recover Array[U32] end
    let n2m2: Array[U32] iso = recover Array[U32] end
    let n3m1: Array[U32] iso = recover Array[U32] end
    let n3m2: Array[U32] iso = recover Array[U32] end

    for v in m1.slice(0, (newrow1 * col).usize()).values() do
        n1m1.push(v)
    end
    for v in m2.slice(0, (newrow1 * col).usize()).values() do
        n1m2.push(v)
    end
    for v in m1.slice((newrow1 * col).usize(), ((newrow1 * col) + (newrow2 * col)).usize()).values() do
        n2m1.push(v)
    end
    for v in m2.slice((newrow1 * col).usize(), ((newrow1 * col) + (newrow2 * col)).usize()).values() do
        n2m2.push(v)
    end
    for v in m1.slice(((newrow1 * col) + (newrow2 * col)).usize(), (row * col).usize()).values() do
        n3m1.push(v)
    end
    for v in m2.slice(((newrow1 * col) + (newrow2 * col)).usize(), (row * col).usize()).values() do
        n3m2.push(v)
    end
    
    Node(env, newrow1, col, consume n1m1, consume n1m2, operation, 1)
    Node(env, newrow2, col, consume n2m1, consume n2m2, operation, 2)
    Node(env, newrow3, col, consume n3m1, consume n3m2, operation, 3)
    env.out.print(" ")

    else
        env.out.print("Either improper operation or improper amount of arguments")
    end

    fun transpose(env: Env, m : Array[U32], row : U32, col : U32) =>
        var x: U32 = 0
        var size: USize = (row*col).usize()
        var n: Array[U32] = []

        var h: U32 = 0
        while h < size.u32() do
            n.push(0)
            h = h + 1
        end

        while x < row do
            var y: U32  = 0
            while y < col do
                var g: USize = ((x * col) + y).usize()
                try n.update(((y * row) + x).usize(), m.apply(g)?.u32())? end
                y = y + 1
            end
            x = x + 1
        end
        env.out.print(row.string() + " " + col.string())
        var count: U32 = 0
        while count < size.u32() do
            if (((count %% row) == 0 ) or (count == 0)) then  
                if (count != 0) then
                    env.out.print("")
                end
            else
               try  env.out.write(n.apply(count.usize())?.string() + " ") end
            end
            count = count + 1
        end
        env.out.print("")

/* LEAVE THIS OUT UNLESS WE HAVE TIME - THIS IS PART OF DETERMINANT
    fun determinant(env: Env, m : Array[U32], row : U32, col : U32): U32 =>
        determinantHelper(m, row)
    
    fun determinantHelper(m : Array[U32], n : U32): U32 =>
        var d: U32 = 0
        if(n == 1) then
            try m.apply(0)? end
        end

        var tmp: Array[U32] = []

        var sign: U32 = 1

        var f: U32 = 0

        while (f < n) do
            getCofactor(m, tmp, 0, f, n)
            try d = d + ((sign * m.apply(f.usize())?) * determinantHelper(tmp, (n -1))) end

            sign = -1 * sign
            f = f + 1
        end
        d

    // next step is to finish this function below
    fun getCofactor(m: Array[U32], tmp: Array[U32], p: U32, q: U32, n: U32 ) =>
        p
    
    
        var i : U32 = 0
        var j : U32 = 0

        var r : U32 = 0
        while r < n do
            var c: U32 = 0
            while c < n do
                if ((r != p) and (col != q)) then
                    tmp.update(((i * n) + j).usize(), m.apply(((r * n) + c)?.usize()))
                    j = j + 1

                    if( j == (n -1)) then
                        j = 0
                        i = i + 1
                    end
                end

                c = c + 1
            end
            r = r + 1
        end
    */

// https://patterns.ponylang.io/async/actorpromise#discussion to bring it all together to print