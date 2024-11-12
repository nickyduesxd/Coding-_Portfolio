/*
Caleb Koutrakos, Nicholas Zayfman
SI413
These are the resources we used:
- https://github.com/ponylang/ponyc/blob/main/examples/mixed/main.pony
- https://patterns.ponylang.io/data-sharing/copying#solution
- https://ponylang.zulipchat.com/#narrow/channel/189985-beginner-help/topic/How.20do.20I.20wait.20for.20an.20actor.20to.20finish.3F
- https://tutorial.ponylang.io/
*/

// Import Libraries
use "files"
use "time"

// This is the Node actor. It takes in smaller "matrices" from the main node and conducts operations on its smaller part. This is 
// the concurrent part of our code. Currently, the operations supported by concurrency in this project are add, sub, scalmul, and mul.
actor Node

    // Declare variables used in this actor.
    var _env: Env
    var _row: U32
    var _col: U32
    var _m1: Array[I32] iso
    var _m2: Array[I32] iso
    var _m3: Array[I32] = []
    var _operation: String
    var _nodeNum: U32

    // Below is the create call, which initializes the variables used in this actor.
    new create(env: Env, row: U32, col: U32,  m1: Array[I32] iso, m2: Array[I32] iso, operation: String, nodeNum: U32) =>
        _env = env
        _row = row
        _col = col
        _m1= consume m1
        _m2 = consume m2
        _operation = operation 
        _nodeNum = nodeNum

    // This is the run behavior that executes once given the message to execute by the main actor. This behavior checks the
    // operation inputed and runs the reference function to execute that operation on the provide matrix. 
    be run() =>
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

    // Function that adds the smaller matrices.
    fun ref add() =>
        var count: U32 = 0
        var size: U32 = _row * _col
        while count < size do
            var value: I32 = 0
            try value = _m1.apply(count.usize())? + _m2.apply(count.usize())? end
            _m3.push(value)
            count = count + 1
        end

    // Function that subtracts the smaller matrices.
    fun ref sub() =>
        var count: U32 = 0
        var size: U32 = _row * _col
        while count < size do
            var value: I32 = 0
            try value = _m1.apply(count.usize())? - _m2.apply(count.usize())? end
            _m3.push(value)
            count = count + 1
        end

    // Function that multiplies the smaller matrices.
    fun ref mul() =>
        var count: U32 = 0
        var size: U32 = _row * _col
        while count < size do
            var value: I32 = 0
            try value = _m1.apply(count.usize())? * _m2.apply(count.usize())? end
            _m3.push(value)
            count = count + 1
        end

    // This is the behavior that prints the new matrix after it gets called from the main actor.
    be print() => 
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

//Below is the Main actor that reads the user input from the Command Line, divides the Matrix into parts, create the nodes, and calls the 
// node's print behavior.
actor Main
	new create(env: Env) =>

    // Declares and Initializes all the variables used in this actor.
    var operation: String = ""
    var col: U32 = 0
    var row: U32 = 0
    var m1: Array[I32] = []
    var m2: Array[I32] = []

    // Reads the operation from the command line
    if ((env.args.size().u32() == 4) or (env.args.size().u32() == 3)) then
        try operation = env.args(1)? end

        var file_name : String = ""

        // Determinant, adj, and tranpose can only take one matrix
        if( (((operation == "transp") or (operation == "det")) or (operation == "adj")) and (env.args.size().u32() == 3)) then
            // Read the matrix from the file.
            try file_name = env.args.slice(2).apply(0)?.string() end
            let path = FilePath(FileAuth(env.root), file_name)
            match OpenFile(path)
        |   let file: File =>
                let allin: String = file.read_string(1024)
                let delimiter: String = " \n"
                let split_array: Array[String] = allin.split(delimiter)
                try 
                col = split_array.apply(0)?.u32()?
                row = split_array.apply(1)?.u32()?
                var count: U32 = 2
                while count <= (split_array.size().u32()) do
                    m1.push(split_array.apply(count.usize())?.i32()?)
                    count = count + 1
                end
                end
                // THe functions only take a square matrix
                if(row == col) then
                if (operation == "transp") then
                    transpose(env, m1, row , col )
                elseif(operation == "adj") then
                    adjoint(env, m1, row)
                elseif (operation == "det") then
                    determinant(env, m1, row, col)
                end
                else
                    env.out.print("Rejected: Must be a square matrix.")
                end
            else
                env.err.print("Error opening file '" + file_name + "'")
            end
        /// For the add, subtract, matrix multiply, and scalar multiply functions, 2 matrices are required.
        elseif((((operation == "add") or (operation == "sub")) or (operation == "mul") or (operation == "scalmul")) and (env.args.size().u32() == 4)) then
            // The code belows opens the file and reads the matrix.
            try file_name = env.args.slice(2).apply(0)?.string() end
            let path = FilePath(FileAuth(env.root), file_name)
            match OpenFile(path)
        |   let file: File =>
                let allin: String = file.read_string(1024)
                let delimiter: String = " \n"
                let split_array: Array[String] = allin.split(delimiter)
                try 
                col = split_array.apply(0)?.u32()?
                row = split_array.apply(1)?.u32()?
                var count: U32 = 2
                while count <= (split_array.size().u32()) do
                    m1.push(split_array.apply(count.usize())?.i32()?)
                    count = count + 1
                end
                end
            else
                env.err.print("Error opening file '" + file_name + "'")
            end
            // Creates a 2nd matrix of just all values of the scalar so we can multiply concurrently.
            if (operation == "scalmul") then
                var count: U32 = 0
                var scal: I32 = 0
                try scal = env.args(3)?.i32()? end
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
                    while count <= (split_array.size().u32()) do
                        m2.push(split_array.apply(count.usize())?.i32()?)
                        count = count + 1
                    end
                end            
            else
                env.err.print("Error opening file '" + file_name + "'")
            end
            end
        env.out.print(row.string() + " " + col.string())
    

        // The section of code divides the matrices into about equal parts.
        var newrow1: U32 = row/3 
        var newrow2: U32 = row/3
        var newrow3: U32 = row/3

        if (newrow1*3) < row then
            newrow1 = newrow1 + 1
        end

        if (newrow1 + (newrow2*2)) < row then
            newrow2 = newrow2 + 1
        end
    
        //Here we declare the arrays the will store the broken down big array. The iso and recover are used for permission purposes
        // for when we pass the arrays to the Node actor.
        let n1m1: Array[I32] iso = recover Array[I32] end
        let n1m2: Array[I32] iso = recover Array[I32] end
        let n2m1: Array[I32] iso = recover Array[I32] end
        let n2m2: Array[I32] iso = recover Array[I32] end
        let n3m1: Array[I32] iso = recover Array[I32] end
        let n3m2: Array[I32] iso = recover Array[I32] end

        //Below we place the divided data into its respective array.
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

        // Creating 3 instances of the Node actors.
        let n1 = Node(env, newrow1, col, consume n1m1, consume n1m2, operation, 1)
        let n2 = Node(env, newrow2, col, consume n2m1, consume n2m2, operation, 2)
        let n3 = Node(env, newrow3, col, consume n3m1, consume n3m2, operation, 3)

        // Runs the operation on their respective matrices.
        n1.run()
        n2.run()
        n3.run()

        // For printing we run into a race condition, 3 different actors trying to print at the same time so often
        // the printed matrix looks funky. We resolved this by creating a timer (another actor) that counts down
        // an increment of time set and at the end, calls the print function in its designated node.
        let timers = Timers
        let timer = Timer(NumberGenerator(env, n1), 5_000_000_000, 5_000_000_000)
        timers(consume timer)

        let timers1 = Timers
        let timer1 = Timer(NumberGenerator(env, n2), 10_000_000_000, 10_000_000_000)
        timers1(consume timer1)

        let timers2 = Timers
        let timer2 = Timer(NumberGenerator(env, n3), 15_000_000_000, 15_000_000_000)
        timers2(consume timer2)

        let timers3 = Timers
        let timer3 = Timer(NumberGenerator1(env), 20_000_000_000, 20_000_000_000)
        timers3(consume timer3)

        else
            env.out.print("> Either improper operation or improper amount of arguments for operation")
            env.out.print("> Operations which require 2 Matrix Files: <add> <sub> <mul> <scalmul>")
            env.out.print("> Operations which require 1 Matrix File: <transp> <det> <adj> ")
            end
    else
        env.out.print("> Improper amount of arguments")
        env.out.print("> Program only accepts 2 or 3 arguments")
        env.out.print("> Usage: ./Phase2 <operation> <Matrix.txt> <Matrix.txt> or Usage: ./Phase2 <operation> <Matrix.txt>")
    end
    // Tranpose functions basically rotates the matrix.
    fun transpose(env: Env, m : Array[I32], row : U32, col : U32) =>
        var x: U32 = 0
        var size: USize = (row*col).usize()
        var n: Array[I32] = []

        var h: U32 = 0
        while h < size.u32() do
            n.push(0)
            h = h + 1
        end

        // Switching values into new array.
        while x < row do
            var y: U32  = 0
            while y < col do
                var g: USize = ((x * col) + y).usize()
                try n.update(((y * row) + x).usize(), m.apply(g)?.i32())? end
                y = y + 1
            end
            x = x + 1
        end
        // Prints output to screen.
        env.out.print(row.string() + " " + col.string())
        var count: U32 = 0
        while count < size.u32() do
            if (((count %% row) == 0 ) or (count == 0)) then  
                if (count != 0) then
                    env.out.print("")
                end
            end
               try  env.out.write(n.apply(count.usize())?.string() + " ") end
            count = count + 1
        end
        env.out.print("")
    // Determinant function is recursive.
    fun determinant(env: Env, m : Array[I32], row : U32, col : U32) =>
        env.out.print(determinantHelper(m, row).string())
    
    fun determinantHelper(m : Array[I32], n : U32): I32 =>
        var det: I32 = 0
        // Base cases
        if(n == 1) then
            try det = m.apply(0)? end
            det
        elseif (n == 2) then
            try det = (m.apply(0)? * m.apply(3)?) - (m.apply(2)? * m.apply(1)?) end
            det

        else 
        //Recursive case
        var tmp: Array[I32] = []

        var count: U32 = 0
        while(count < ((n-1)*(n-1))) do
            tmp.push(0)
            count = count + 1
        end
        
        count = 0
        var sign: I32 = 0
        while(count < n) do
            tmp = getCofactor(m, tmp, n, 0, count)
            var t: U32 = 0

            // Determinant formula: det = sum((-1)^col * matrix[0][col] * det(cofactor))
            if((count %% 2) == 0) then
                sign = 1
            else
                sign = -1
            end

            try det = det +  (sign * ( m.apply(count.usize())?  * (determinantHelper(tmp, n -1)))) end

        
            count = count + 1
        end
        det
        end
    // Part of determinant function and adjoint function.
    fun getCofactor(m: Array[I32], tmp: Array[I32], n: U32, row: U32, col: U32): Array[I32] =>
        var i: U32 = 0
        var j: U32 = 0

        var r: U32 = 0
        var c: U32 = 0

        while(r < n) do
            c = 0
            while(c < n) do
                if((r != row) and (c != col)) then
                    try tmp.update(((i*(n-1))+ j).usize(), m.apply(((r*n)+ c).usize())?)? end 
                    j = j +1
                    if(j == (n-1)) then
                        j = 0
                        i = i + 1
                    end
                end
                c = c + 1
            end
            r = r + 1
        end
        tmp

    // Works only up to 2 by 2 matrices.
    fun adjoint(env: Env, m: Array[I32], n: U32) =>
        var size: U32 = n * n
        var sign: I32 = 0
        var i: U32 = 0
        var j: U32 = 0
        var adj: Array[I32] = []
        var tmp: Array[I32] = []
        var adj2: Array[I32] = []
        //var adj: Array[I32] = []

        if(size == 1) then
            adj.push(1)
            transpose(env,adj, 1, 1)
        else
            var count: U32 = 0
            while(count < (n*n)) do
                tmp.push(0)
                adj.push(0)
                count = count + 1
            end
            while (j < n) do
                i = 0
                while (i < n ) do 

                    tmp = getCofactor(m, tmp, n, j, i)
                    if(((i+j)%% 2) == 0) then
                        sign = 1
                    else
                        sign = -1
                    end
                    try adj.update(((i*n) + j ).usize(), (sign * determinantHelper(tmp, n-1)))? end //n -1 after temp
                    i = i + 1
                end
                j = j + 1
            end
            transpose(env, adj, n, n)
        end

// This class is a timer that when reaches the end it calls the print function of the node.
// It is used to prevent the racing condition for printing to one environment from
// different actors.
class NumberGenerator is TimerNotify
  let _env: Env
  let _n: Node
  var _counter: U64

  new iso create(env: Env, n: Node) =>
    _n = n
    _counter = 0
    _env = env

  fun ref apply(timer: Timer, count: U64): Bool =>
    _n.print()
    false

// This clas is a timer as well and was specifically created in order to print a new line
// at the end of the nodes printing.
class NumberGenerator1 is TimerNotify
  let _env: Env
  var _counter: U64

  new iso create(env: Env) =>
    _counter = 0
    _env = env

  fun ref apply(timer: Timer, count: U64): Bool =>
    _env.out.print("")
    false