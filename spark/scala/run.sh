spark-submit \
    --conf spark.task.maxFailures=1 \
    --conf spark.yarn.max.executor.failures=1 \
    --conf spark.port.maxRetries=10 \
    --class  "SparkApp"\
    --master local \
    --deploy-mode client \
    --driver-memory 1g \
    --executor-memory 1g \
    --executor-cores 1 \
    --num-executors 1 \
    --conf spark.sql.shuffle.partitions=973 \
    --conf spark.default.parallelism=973 \
    --conf spark.dynamicAllocation.enabled=false \
    target/scala-2.12/spark-app_2.12-1.0.jar \
    1>logs/stdout 2>logs/stderr

#--driver-java-options "-Dlog4j.configuration=file:${currentDir}/conf/log4j.properties" \
