1. compile: bazel build //main:hello-world （//main:是寻找BUILD文件所在的位置，hello-world是要编译的target，target见5）
2. run: ./bazel-bin/main/hello-world
3. workspace：包含WORKSPACE的目录为一个workspace，这是一个工程的根目录，这里是my-project。
4. package：包含BUILD文件的目录为一个package，这是工程的基本组成单位。
5. BUILD文件由各种targets组成，bazel使用BUILD编译源文件。

参考:
1. http://blog.163.com/wujiaxing009@126/blog/static/71988399201751110532137/
2. https://docs.bazel.build/versions/master/tutorial/cpp.html
