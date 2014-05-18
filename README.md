#Pointer Access Tests#
`std::vector<Obj>` vs `std::vector<std::shared_ptr<Obj>>`

described on a blog post at [Code And Graphics - Vector of object vs Vector of pointers Tests](http://www.bfilipek.com/2014/04/vector-of-obj-vs-vector-of-ptr.html)

> After watching some of the talks from **[Build 2014](http://www.buildwindows.com/)** - especially ["Modern C++: What You Need to Know"](http://channel9.msdn.com/Events/Build/2014/2-661) and some talks from [Eric Brumer](http://channel9.msdn.com/Events/Speakers/eric-brumer) I started thinking about writing my own test case. Basically I've created a simple code that compares `vector<Obj>` vs `vector<shared_ptr<Obj>>`. The first results are quite interesting so I thought it is worth to describe thin on the blog. I also think that such test can help me in [**my particle experiments**](http://www.bfilipek.com/2014/04/flexible-particle-system-container.html)...
