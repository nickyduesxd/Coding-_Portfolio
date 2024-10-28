actor Main
    new create(env: Env) =>
        let x: Foo = Foo.create(1)
        env.out.print(x.retTest().string())

actor Foo
    var y : U32
    new create(x : U32) =>
        y = x
    
    fun retTest(): U32 =>
        y