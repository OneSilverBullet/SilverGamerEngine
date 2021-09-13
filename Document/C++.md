
## C++

### 第一讲

#### 构造函数
* 关于默认构造函数的构造过程： 
  
（1） 调用基类构造函数
  
（2）构造虚表。

（3）按照顺序调用成员的构造函数。

（4）如果是triVial type，那么忽略掉。

* 拷贝构造函数

（1）分为两种类型：bitwise和memberwise。

（2）Bitwise函数更快一些，triviallyCopyable=bitwise copy=memcpy

（3）memberwise的拷贝构造函数运行过程与默认构造函数一致。

（4）只要不是编译器生成的，都是memberwise的拷贝构造函数。

* 更快的拷贝方案：copy elision 拷贝消除

（1）定义：在拷贝构造函数的时候进行优化。

（2）Return Value Optimization:直接返回的就是目标的值。

（3）Named Return Value Optimization:返回局部对象。

```
void test_nrvo()
{
    Foo tmp;
    return tmp;
}

//如果没有拷贝消除
//rvo调用三次拷贝构造函数：Constructed Move-Constructed Move-Constructed
//rvo在拷贝消除下，调用一次拷贝构造函数，直接在目标地址生成拷贝对象。
void test_rvo()
{
    return Foo();
}
```

（4）拷贝消除的实现：直接在返回值的地址上进行构建。



* member initialize

（1）对象的初始化顺序是按照声明顺序进行的。

（2）Default member initializer:在变量初始化的时候就定义这些值。不能够初始化指针，可能会导致循环引用。

* 类型转换

（1）可以在类中定义其他类型的operator OtherClass()，实现类型转换。

（2）通过explicit来控制显式、隐式。

* 析构函数

（1）虚析构函数：如果需要使用基类指针管理派生类，那么需要虚析构函数。

* static 周期变量初始化顺序

（1）在调用main前调用这些变量。

（2）跨翻译单元：未定义的方案。

（3）相同翻译单元：按照定义顺序。

（4）释放的顺序与声明顺序一致。

#### 数据成员

* 数据成员标识符

（1） mutable 

（2） static

* bitfield

(1)定义：bitwise usage of memory。

* class layout & sub-object

* 空基类优化：空类具有一个字节，是为了检测是否具有相同地址。

（1）继承自空基类，这一个字节会被优化掉。


#### 成员函数

注意：成员函数与普通函数的区别是，成员函数具有this指针。

* cv修饰符：const volatile。

* unqualified->(const、 volatile)->const volatile

* const qualifier

（1）使得实体成员是const。

（2）const实例只能访问const成员函数。

（3）代码复用：使用类型转换来实现。

（4）volatile：保证每一次都从实际内存中去取，不能进行优化。

* 将基类函数引入：using Base::print

* 删除成员函数 = delete

* 跨继承层级的函数调用

（1）virtual在第一次定义成员函数的时候使用。

（2）override在继承类里使用。

（3）final在最终一次继承中使用。


* 成员函数默认参数，其数值可以使用static const的成员作为成员函数的形参默认值。


#### 名字查找

* 概述:尝试解析一个函数调用的过程。

（1）名字查找

（2）重载解析：选择一个最匹配的重载。

（3）模糊调用：如果有超过一个最匹配，那么报错。

* 名字查找

（1）定义：当编译器找到一个名字时，找到一个最匹配的定义来解释它。

（2）Unqualified Name Lookup: 不加命名空间的名字查找。block scope->class scope->base class scope->enclosing namespace->util global scope

（3）Qualified Name Lookup: 加命名空间的名字查找。

* argument dependent lookup

（1）定义：参数依赖检查。
































































































