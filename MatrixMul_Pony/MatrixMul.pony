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


// Node class
class Node
    let _env: Env
    new create(env: Env) =>
        _env = env

    var row: U64 = 20
    var col: U64 = 20
    var size: U64 = row * col
    let a = Array[U32](size.usize())
    let b = Array[U32](size.usize())
//class UserInput is ReadlineNotify


/*
    //Define global variables
    var row: U64 = 0
    var col: U64 = 0
    var print: U64 = 0
    var input_seed: U64 = 0
    var test: String = ""
    // Usage ./Phase 2 <row> <col> <seed> <print> <algorithm>

    let term = ANSITerm(Readline(recover UserInput(_env.out) end, _env.out), _env.input)
    _env.out.print("usage: <row> <col> <print>")
    _env.out.print("<row> : Integer")
    _env.out.print("<col> : Integer")
    

    term.prompt("")
    let notify = object iso
        let term: ANSITerm = term
        fun ref apply(data: Array[U8] iso) => term(consume data)
        fun ref dispose() => term.dispose()
    end
    _env.input(consume notify)

    // Reads input from user then runs matrix mul
    //fun run_algorithm(inp: String) =>
    //    _env.out.print(inp.string())

    class UserInput is ReadlineNotify
        let _out: OutStream

        new create(out: OutStream) =>
            _out = out
   //this is run whenever UserInput object is created
        fun ref apply(line: String, prompt: Promise[String]) =>
            //try
                run_algorithm(line)
            //end
            //    prompt.reject()
        fun ref run_algorithm(inp: String) =>
            test = inp
            _out.print(inp.string())
            //end
            */