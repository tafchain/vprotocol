---
#  编译OpenSsl库
1. 设置环境变量
> 在 .profile 中设置:   
> ```export SSL_ROOT=$HOME/tools/openssl```     
> ```export LD_LIBRARY_PATH=$SSL_ROOT/lib:$LD_LIBRARY_PATH```  

2. 下载openssl压缩包(以openssl-1.1.1c.tar.gz为例)，进入解压后目录，执行命令  
> ```./config --prefix=$SSL_ROOT```

3. 打开Makefile文件，执行如下修改：  
> 找到变量 ```SHLIB_EXT``` 定义的位置，将其值修改为 ```SHLIB_EXT=vbh.so```  
> 找到 -O3 在前面加上-mtune=native -march=native -flto
4. 执行编译命令即可 make && make install



---
# 编译开关说明

## 通用开关
1. ```plat``` 开关用于选择平台。可以选择的取值:  
> * linux  
> * android
> * ios
> * freebsd
> * __无默认值，编译时必须指定__

2. ```release``` 开关用于选择是否编译为发行版本。可以选择的取值为:
> * 1 编译发行版本
> * 0 编译调试版本; __默认值__


## linux环境特有开关(plat=linux)
1. ```native``` 开关用于选择是否针对本机的处理器进行优化。
> * 1  优化
> * 0  不优化 __默认值__
> * __说明:__ 只有```release=1```时，```native=1```才生效


# andrid环境特有开关(plat=android)

---
# 编译前环境准备
1. 将 conf/profile.sh中的内容拷贝到 ~/.profile 或 ~/.bash_profile后，执行 source ~/.profile 或 source ~/.bash_profile后，才可以执行make相关命令。
2. linux下编译前需要安装的库：yum install libuuid-devel