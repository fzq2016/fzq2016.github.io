# dar
# drawLine 绘制线段

### 绘制线段步骤
1. 新建一个类，继承自UIView（略）

2. 在-(void)drawRect:(CGRect)rect方法实现下述几步（略）

3. 取得跟当前view相关联的图形上下文

4. 拼接路径，绘制相应的图形内容

- 把路径添加到上下文

- 设置绘图状态

- 利用图形上下文将绘制的所有内容渲染显示到view上面

### 获取图形上下文
- CG:表示这个类在CoreGraphics框架里  Ref:引用
- 目前学的上下文都跟UIGraphics有关，想获取图形上下文，首先敲UIGraphics。
```objc
CGContextRef ctx = UIGraphicsGetCurrentContext();
```

### 拼接路径

```objc
### 画一根线
    //创建贝塞尔路径
    UIBezierPath *path = [UIBezierPath bezierPath];

    // 2.1 设置起点
    [path moveToPoint:CGPointMake(10, 100)];

    // 添加线到终点
    [path addLineToPoint:CGPointMake(200, 200)];

//再添加一根线
        直接addLineToPoint，因为路径是拼接的，默认下一条线的起点是上一条线的终点。

    [path addLineToPoint:CGPointMake(100, 100)];


### 画两跟不连接的线
        //1. 第二次画的时候，重新设置起点，然后画线。一个路径可以包含多条线段。
        //2. 新创建一个路径，添加到上下文。开发中建议使用这种，比较容易控制每根线。
    //    path = [UIBezierPath bezierPath];
    //
    //    // 设置起点,移动到某个位置
    //    [path moveToPoint:CGPointMake(200, 200)];
    //
    //    // 添加一根线到某个点
    //    [path addLineToPoint:CGPointMake(100, 200)];

### 画曲线
        - 3个点，起点，终点，控制点。
```

### 把路径添加到上下文
- CGPath转换：UIKit框架转CoreGraphics直接使用CGPath属性就能转
```objc
CGContextAddPath(ctx, path.CGPath);
```

### 设置绘图状态
- 绘图状态调用顺序：只要在渲染之前就好了，在渲染的时候才会去看绘图的最终状态。
```objc
    // 设置绘图状态,描述线段信息
    CGContextSetLineWidth(ctx, 10);//线段宽度
    // 设置圆角顶角样式
    CGContextSetLineCap(ctx, kCGLineCapRound);

    CGContextSetLineJoin(ctx, kCGLineJoinRound);
```

### 把上下文渲染到视图
```objc
CGContextStrokePath(ctx);
```



##贝塞尔路径
- 绘制图形一般开发中用贝塞尔路径，里面封装了很多方法，可以帮我画一些基本的线段，矩形，圆等等
- 使用贝塞尔路径方法可以省略`获取上下文`和`把路径加入到上下文`两步；上述代码也可以这样简单实现：

### 通过贝塞尔路径绘制线段步骤
1. 新建一个类，继承自UIView（略）

2. 在-(void)drawRect:(CGRect)rect方法实现下述几步（略）

- 拼接路径，绘制相应的图形内容

- 设置绘图状态（设置path.的属性）

- 调用贝塞尔路径方法渲染内容

```objc
// 描述路径
    UIBezierPath *path = [UIBezierPath bezierPath];

    [path moveToPoint:CGPointMake(50, 50)];

    [path addLineToPoint:CGPointMake(100, 100)];

    //设置绘图状态
    path.lineWidth = 10;//通过设置path的属性来设置

    // 渲染
    //渲染是使用贝塞尔路径的方法还是使用CG函数，具体看怎么设置绘图状态，混用会有bug
    //如果通过设置path的属性（使用贝塞尔路径封装好的方法，stroke,fill等）。
    //如果通过CGContextSetLineWidth等方法，则用CGContextStrokePath(ctx)等加入上下文
    [path stroke];

    // 填充方法:将区域颜色填充，但不会画上区域边线
//    [path fill];

```
