set -e

g++ test_libhdfs.cpp -o test_libhdfs -I /home/appops/hadoop-2.7.3/include/ -L /home/appops/hadoop-2.7.3/lib/native/ -L /usr/lib/jvm/java-7-openjdk-amd64/jre/lib/amd64/server -lhdfs -ljvm


CLASSPATH=${JAVA_HOME}/lib:${JRE_HOME}/lib
CLASSPATH=${CLASSPATH}":"`find ${HADOOP_HOME}/share/hadoop | awk '{path=path":"$0}END{print path}'`
export CLASSPATH

export LD_LIBRARY_PATH=/home/appops/hadoop-2.7.3/lib/native:/usr/lib/jvm/java-7-openjdk-amd64/jre/lib/amd64/server:$LD_LIBRARY_PATH
./test_libhdfs
