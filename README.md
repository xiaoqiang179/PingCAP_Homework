## PingCAP小测试
### 问题:给定100GB的URL,求出出现最频繁的前100个URL,要求是电脑的内存只有1GB.还要给出出现次数.
我的解决方案:用的是C&C++,大致思路是这样的,首先要把每个URL分类,当然这些暂存都不能放在内存中,我的选择是将URL哈希值%5000得出的值作为一个下标,然后暂存url到以下标为名的文件中,此过程为O(n)复杂度(将hash看作O(1)).读取总的数据采取的是一次读1G的数据,然后处理每个URL将其放入相应文件,此过程是最费时的过程,因为涉及到将每个1G的buffer分割成每个URL且要将URL存到对应文件.处理完后即可开始统计,统计采用的是求出每个文件的TOP100然后再取总的TOP100,此过程为O(n*log100),最终输出结果.

如何测试:repo里是Xcode项目,若没有Xcode就用Clion,直接在终端运行就打不开那个生成的文件.不知道为啥.
首先可以在```fileGeneratorNGB(1);```指定文件大小.然后看输出即可.

下面是几个测试结果:

我的测试平台:

```
MacBook Pro 2017 15寸的低配
CPU: Core i7 7700HQ
RAM: 16 GB 2133 MHz LPDDR3
硬盘是固态,这个很影响结果.
我的剩余空间只有50GB了所以最多
到25GB的数据,但看时间走势,100GB应该能在10min内拿下

```
这是图 加载不出来可以下载那个图看

![](https://github.com/xiaoqiang179/PingCAP_Homework/blob/master/%E6%88%AA%E5%B1%8F2020-04-26%2019.29.22.png)
![](https://github.com/xiaoqiang179/PingCAP_Homework/blob/master/%E6%88%AA%E5%B1%8F2020-04-27%2014.11.17.png)
