# breakpadDemo
breakpad demo

上传dmp文件到sentry，需要命令行指定一个sentry url

url可在sentry项目里参照项目设置中Security Headers中的REPORT URI填写，注意做如下修改：
1. security改为minidump
2. api前面只保留一个/

最后类似这样：
```
http://10.224.18.144:9000/api/3/minidump/?sentry_key=3edeb8d3f27c4eb38e4220232f1c276f
```
