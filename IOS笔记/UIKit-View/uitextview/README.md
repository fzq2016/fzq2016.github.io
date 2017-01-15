## UITextView（文本视图）
- 父类是UIScrollView

### UITextField常见属性
```objc
//  设置文本框文字内容
textView.text = @"文字";

//  是否可以编辑
textView.editable = NO;
```

### 根据字符串计算UITextView高度
1. 注意：UITextView在上下左右分别有一个8px的padding，如果不是采用系统方法（sizeThatFits，见方法1），手动计算需要将UITextView.contentSize.width减去16像素（左右的padding 2 x 8px）。同时返回的高度中再加上16像素（上下的padding），这样得到的才是UITextView真正适应内容的高度。（见方法2，3）

2.  方法一：通用（推荐）

```objectivec
/**

 @method
 获取指定宽度width的字符串在UITextView上的高度

 @param
 textView 待计算的UITextView

 @param
 Width 限制字符串显示区域的宽度

 @result
 float 返回的高度

 */

-  (float)heightForString:(UITextView *)textView andWidth:(float)width{

     CGSize sizeToFit = [textView sizeThatFits:CGSizeMake(width, MAXFLOAT)];

    return sizeToFit.height;
}
```

3.方法二：ios7.0及之后适用

```objectivec
/**

 @method
 获取指定宽度width,字体大小fontSize,字符串value的高度

 @param
 value 待计算的字符串

 @param
 fontSize 字体的大小

 @param
 Width 限制字符串显示区域的宽度

 @result
 float 返回的高度

 */

- (float)heightForString:(NSString *)value andWidth:(float)width{

    // 获取当前文本的属性
    NSAttributedString *attrStr = [[NSAttributedString alloc] initWithString:value];

    _text.attributedText = attrStr;

    NSRange range = NSMakeRange(0 , attrStr.length);

    // 获取该段attributedString的属性字典

    NSDictionary *dic = [attrStr attributesAtIndex:0
													effectiveRange:&range];

    // 计算文本的大小
	// Size：用于计算文本绘制时占据的矩形块
	// options：文本绘制时的附加选项
	// attributes：文字的属性
	// context上下文。包括一些信息，例如如何调整字间距以及缩放。该对象包含的信息将用于文本绘制。该参数可为nil
    CGSize sizeToFit = [value boundingRectWithSize:CGSizeMake(width - 16.0,
 MAXFLOAT)
						options:NSStringDrawingUsesLineFragmentOrigin
 | NSStringDrawingUsesFontLeading 
 					 attributes:dic       
						context:nil].size;

    return sizeToFit.height + 16.0;
}
```	

4.方法三：ios7.0之前适用
```objectivec
/**

 @method
 获取指定宽度width,字体大小fontSize,字符串value的高度

 @param
 value 待计算的字符串

 @param
 fontSize 字体的大小

 @param
 Width 限制字符串显示区域的宽度

 @result
 float 返回的高度

 */

- (float)heightForString:(NSString *)value fontSize:(float)fontSize
 andWidth:(float)width

{
	CGSize sizeToFit = [value sizeWithFont:[UIFont systemFontOfSize:fontSize] 
						 constrainedToSize:CGSizeMake(width
 -16.0,CGFLOAT_MAX)
							 lineBreakMode:NSLineBreakByWordWrapping];

        //此处的换行类型（lineBreakMode）可根据自己的实际情况进行设置

    return sizeToFit.height + 16.0;
}
```
