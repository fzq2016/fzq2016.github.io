## https
###6.HTTPS的基本使用

```objc
1.https简单说明
    HTTPS（全称：Hyper Text Transfer Protocol over Secure Socket Layer），是以安全为目标的HTTP通道，简单讲是HTTP的安全版。
    即HTTP下加入SSL层，HTTPS的安全基础是SSL，因此加密的详细内容就需要SSL。 它是一个URI scheme（抽象标识符体系），句法类同http:体系。用于安全的HTTP数据传输。
    https:URL表明它使用了HTTP，但HTTPS存在不同于HTTP的默认端口及一个加密/身份验证层（在HTTP与TCP之间）。

2.HTTPS和HTTP的区别主要为以下四点：
        一、https协议需要到ca申请证书，一般免费证书很少，需要交费。
        二、http是超文本传输协议，信息是明文传输，https 则是具有安全性的ssl加密传输协议。
        三、http和https使用的是完全不同的连接方式，用的端口也不一样，前者是80，后者是443。
        四、http的连接很简单，是无状态的；HTTPS协议是由SSL+HTTP协议构建的可进行加密传输、身份认证的网络协议，比http协议安全。

3.对开发的影响。
3.1 如果是自己使用NSURLSession来封装网络请求，涉及代码如下。
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSURLSession *session = [NSURLSession sessionWithConfiguration:[NSURLSessionConfiguration defaultSessionConfiguration] delegate:self delegateQueue:[NSOperationQueue mainQueue]];

    NSURLSessionDataTask *task =  [session dataTaskWithURL:[NSURL URLWithString:@"https://www.apple.com"] completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
        NSLog(@"%@", [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding]);
    }];
    [task resume];
}

/*
 只要请求的地址是HTTPS的, 就会调用这个代理方法
 我们需要在该方法中告诉系统, 是否信任服务器返回的证书
 Challenge: 挑战 质问 (包含了受保护的区域)
 protectionSpace : 受保护区域
 NSURLAuthenticationMethodServerTrust : 证书的类型是 服务器信任
 */
- (void)URLSession:(NSURLSession *)session didReceiveChallenge:(NSURLAuthenticationChallenge *)challenge completionHandler:(void (^)(NSURLSessionAuthChallengeDisposition, NSURLCredential *))completionHandler
{
    //    NSLog(@"didReceiveChallenge %@", challenge.protectionSpace);
    NSLog(@"调用了最外层");
    // 1.判断服务器返回的证书类型, 是否是服务器信任
    if ([challenge.protectionSpace.authenticationMethod isEqualToString:NSURLAuthenticationMethodServerTrust]) {
        NSLog(@"调用了里面这一层是服务器信任的证书");
        /*
         NSURLSessionAuthChallengeUseCredential = 0,                     使用证书
         NSURLSessionAuthChallengePerformDefaultHandling = 1,            忽略证书(默认的处理方式)
         NSURLSessionAuthChallengeCancelAuthenticationChallenge = 2,     忽略书证, 并取消这次请求
         NSURLSessionAuthChallengeRejectProtectionSpace = 3,            拒绝当前这一次, 下一次再询问
         */
//        NSURLCredential *credential = [NSURLCredential credentialForTrust:challenge.protectionSpace.serverTrust];

        NSURLCredential *card = [[NSURLCredential alloc]initWithTrust:challenge.protectionSpace.serverTrust];
        completionHandler(NSURLSessionAuthChallengeUseCredential , card);
    }
}

3.2 如果是使用AFN框架，那么我们不需要做任何额外的操作，AFN内部已经做了处理。
```



1）iOS9中新增App Transport Security（简称ATS）特性, 让原来请求时候用到的HTTP，全部都转向TLS1.2协议进行传输。
2）这意味着所有的HTTP协议都强制使用了HTTPS协议进行传输。
3）如果我们在iOS9下直接进行HTTP请求是会报错。系统会告诉我们不能直接使用HTTP进行请求，需要在Info.plist中控制ATS的配置。
    "NSAppTransportSecurity"是ATS配置的根节点，配置了节点表示告诉系统要走自定义的ATS设置。
    "NSAllowsAritraryLoads"节点控制是否禁用ATS特性，设置YES就是禁用ATS功能。
4）有两种解决方法，一种是修改配置信息继续使用以前的设置。
    另一种解决方法是所有的请求都基于基于"TLS 1.2"版本协议。（该方法需要严格遵守官方的规定，如选用的加密算法、证书等）

/*
 ATS默认的条件
 1)服务器TLS版本至少是1.2版本
 2)连接加密只允许几种先进的加密
 3)证书必须使用SHA256或者更好的哈希算法进行签名，要么是2048位或者更长的RSA密钥，要么就是256位或更长的ECC密钥。
 */

AFSecurityPolicy，内部有三个重要的属性，如下：

AFSSLPinningMode SSLPinningMode;    //该属性标明了AFSecurityPolicy是以何种方式来验证
BOOL allowInvalidCertificates;      //是否允许不信任的证书通过验证，默认为NO
BOOL validatesDomainName;           //是否验证主机名，默认为YES

"AFSSLPinningMode"枚举类型有三个值，分别是AFSSLPinningModeNone、AFSSLPinningModePublicKey、AFSSLPinningModeCertificate。

"AFSSLPinningModeNone"代表了AFSecurityPolicy不做更严格的验证，"只要是系统信任的证书"就可以通过验证，不过，它受到allowInvalidCertificates和validatesDomainName的影响；

"AFSSLPinningModePublicKey"是通过"比较证书当中公钥(PublicKey)部分"来进行验证，通过SecTrustCopyPublicKey方法获取本地证书和服务器证书，然后进行比较，如果有一个相同，则通过验证，此方式主要适用于自建证书搭建的HTTPS服务器和需要较高安全要求的验证；

"AFSSLPinningModeCertificate"则是直接将本地的证书设置为信任的根证书，然后来进行判断，并且比较本地证书的内容和服务器证书内容是否相同，来进行二次判断，此方式适用于较高安全要求的验证。

如果HTTPS服务器满足ATS默认的条件，而且SSL证书是通过权威的CA机构认证过的，那么什么都不用做。如果上面的条件中有任何一个不成立，那么都只能修改ATS配置。
