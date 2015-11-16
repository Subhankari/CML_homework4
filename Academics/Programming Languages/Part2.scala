  class A(x: Int) extends Ordered[A]{ 

    val value = x
      
    def compare(that:A) = if (that.isInstanceOf[B]){
     ((this.value) compare (that.asInstanceOf[B].xvalue + that.asInstanceOf[B].yvalue))} else {this.value compare that.value}
     
    override def toString():String = "A<" + value + ">"
  }
  
  class B(x: Int , y: Int) extends A(x){
    def this(x:Int){
      this(x, 0)
    }
     val xvalue = x
     val yvalue = y    
    
   override def compare(that:A) = if (that.isInstanceOf[B]){
     ((this.xvalue + this.yvalue) compare (that.asInstanceOf[B].xvalue + that.asInstanceOf[B].yvalue))} else {(this.xvalue + this.yvalue) compare that.value
   }
    //override def compare(that:A) =  (this.xvalue + this.yvalue) compare that.value
    
    override def toString():String = "B<" + xvalue + "," + yvalue + ">"
  }
  
  class C[+T](f:Int=>T){
    def apply(x:Int):T = f(x) 
  }  
  
  abstract class Tree[T <: Ordered[T]] extends Ordered[Tree[T]]{
     def insert(x:T):Tree[T]
  }
  case class Leaf[T <: Ordered[T]](v:T) extends Tree[T]{ //with Ordered[Tree[T]]{
    val value = v
    override def toString():String = value.toString()
    override def <(other: Tree[T]) = other match {
    case Leaf(ov) => v < ov
    case _ => false
    }
    def compare(that:Tree[T]) = Leaf(v) compare that 
    def insert(x:T): Tree[T] = if(this.v <= x) new Node(this.v,Empty(),Leaf(x)) else new Node(this.v,Leaf(x),Empty())
  }

    
  case class Node[T <: Ordered[T]](v:T, l:Tree[T], r:Tree[T]) extends Tree[T]{ //with Ordered[Tree[T]]{
    
     override def toString():String = "(" + l + "," + v + "," + r + ")"
     
     override def <(other: Tree[T]) = other match {
      case Node(ov,ol,or) => (v < ov) && (l < ol) && (r < or)
      case _ => false
     }
     def compare(that:Tree[T]) = Node(v,l,r) compare that
     
    def insert(x:T): Tree[T] = if(x < this.v) new Node(v,l.insert(x),r) else new Node(v,l,r.insert(x))

  }
  
  case class Empty[T <: Ordered[T]]() extends Tree[T]{// with Ordered[Tree[T]]{
     
    override def toString():String = ""
    override def <(other: Tree[T]) = other match {
    case Empty() => true
    case _ => false
    
  }
    def compare(that:Tree[T]) = Empty() compare that
    def insert(x:T): Tree[T] = new Leaf(x)
  }
  
  object Part2{
    
    def applyTo10(c:C[A]) = c.apply(10)
    
    def main(args: Array[String]){
      test()
    }
    
    def test() {
      val c1 = new C((x:Int) => new A(x))
      println(c1.apply(3))
  
      val c2 = new C((x:Int) => new B(x+1, x+2))
      println(c2.apply(3))
  
      println(applyTo10(c1))
      println(applyTo10(c2))  //relies on covariant subtyping
  
      var t1: Tree[A] = Empty()
       t1 = t1.insert(new A(4))
       t1 = t1.insert(new A(3))
       t1 = t1.insert(new B(4,1))
       t1 = t1.insert(new A(2))
      println(t1)
  }
    
  }
  
