// Set foundation for reading from user input
// Working on reading a file and return row, col, and array
// additional: functions for each operation also creating output file

use "random"
use "time"
use "term"
use "stdlib"
use "promises"
use "files"

actor Main
    // Create environment
    let _env: Env
    
    new create(env: Env) =>
        _env = env
        //var input: Array[String]
        let term = ANSITerm(Readline(recover UserInput(_env.out) end, _env.out), _env.input)
        term.prompt("How many lines? ")
        let notify = object iso
            let term: ANSITerm = term
            fun ref apply(data: Array[U8] iso) => term(consume data)
            fun ref dispose() => term.dispose()
        end
        _env.input(consume notify)

// Ask user for matrix operation: Scalar multiplication, Matrix Multiplication, transpose, and inverse of the matrix.
// new class for every operation

class UserInput is ReadlineNotify
   let _out: OutStream
   new create(out: OutStream) =>
     _out = out
    
    fun ref readFile(filePath: String): (U32, U32, Array[U32]) =>
        //let array: Array[String] = [; "cat"; "wombat"]
        //try

        // do file shit here format rows cols new line then matrix values
        

        var ret: (U32, U32, Array[U32]) = (5, 7, [2;4;3])
        ret
        
    fun ref apply(line: String, prompt: Promise[String]) =>
        let delimiter: String = " "
        let split_array: Array[String] = line.split_by(delimiter)
        try
            if split_array.apply(0)? == "add" then
                //_out.print(split_array.apply(0)?)
                //_out.print(split_array.size().string()) // it fucking works
                if split_array.size() == 3 then
                    _out.print(readFile(split_array.apply(1)?)._2.string())
                else
                    _out.print("Program stopped because improper number of inputs.")
                end
            elseif split_array.apply(0)? == "sub" then
                _out.print(split_array.apply(0)?)
            elseif split_array.apply(0)? == "mul" then
                _out.print(split_array.apply(0)?)
            elseif split_array.apply(0)? == "transpose" then
                _out.print(split_array.apply(0)?)
            elseif split_array.apply(0)? == "inverse" then
                _out.print(split_array.apply(0)?)
            elseif split_array.apply(0)? == "scalmul" then
                _out.print(split_array.apply(0)?)
            elseif split_array.apply(0)? == "det" then //determinent
                _out.print(split_array.apply(0)?)
            else
                _out.print("Error with matrix operation.")
            end
        end
        //_out.print("OUTPUT:")
        //for value in split_array.values() do
        //    _out.print(value)
        //end

     //end
        prompt.reject() //Closes user input

 