package com.assignment.pkg;
import java.util.*;

	interface Addable<T>{
		T plus (T x);
	}
	
	class MyList<T extends Comparable <T> & Addable<T>> extends ArrayList<T> implements Comparable<MyList<T>>,Addable<MyList<T>>{
		
		private T t;
		
		MyList(T z){
			t = z;
		}
		
		
		public T plus(T x){
			return this.plus(x);
		}
		
		public MyList<T> plus (MyList<T> L){
			Integer index;
			index = this.size();
			this.addAll(index,L);
			return this;
		}
		
		public T sum(MyList <T> L){
			T t1 = t;
			for(Integer i = 0; i < L.size(); i++){
				t1 = L.get(i).plus(t1);
			}
			return t1;
		}
		
		public int compareTo(MyList <T> L){
			return sum(this).compareTo(sum(L));
		}
	
	}
	
	
	class A implements Comparable<A>,Addable<A>{
		Integer a;
		Integer sum;
		A(Integer x){
			a = x;
		}

		public int compareTo(A b){
			if(this.a == b.a){
				return 0;
			}else{
				return this.a > b.a ? 1 : -1;
			}
		}
		
		public A plus(A b){
			int sum;
			sum = this.a + b.a;
			A c = new A(sum);
			return c;
		}
		
		public String toString(){
			return "A<"+a+">";
			
		}
		
	}
	class B extends A implements Comparable<A>,Addable<A>{
		
		B(Integer x){
			super(x);
		}
		public String toString(){
			return "B<"+a+">";
			
		}
		
	}
	
	public class Part1{
		public static void main(String[] args){
			test();
		}
		
		public static void insertIntoMyList(A z , MyList<A> L){
			L.add(z);	
		}
		
		public static void insertIntoMyList(B z, MyList<A> L){
			L.add(z);
		}
		
		public static void test() {
			MyList<A> m1 = new MyList<A>(new A(0));
			MyList<A> m2 = new MyList<A>(new A(0));
			for(Integer i = 1; i <= 5; i++) {
			    insertIntoMyList(new A(i),m1);
			    insertIntoMyList(new B(i),m2);
			}

			insertIntoMyList(new A(6),m2);
			System.out.println("m1 = " + m1);
			System.out.println("m2 = " + m2);
			int result = m1.compareTo(m2);
			String s = (result < 0) ? "less than" : (result == 0) ? "equal to" : "greater than";
			System.out.println("m1 is " + s + " m2");
			System.out.println("m1 + m2 = " + m1.plus(m2));
		    }
	}
	

