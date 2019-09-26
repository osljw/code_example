import org.apache.spark.{ SparkConf, SparkContext }
import org.apache.spark.sql.SparkSession

object SparkApp {
    def main(args: Array[String]) {

        val sparkConf = new SparkConf().setAppName("SparkApp")
        val spark = SparkSession.builder()
            .config(sparkConf)
            .enableHiveSupport()
            .getOrCreate()

        val showSql = "show databases"
        val rdd = spark.sql(showSql)
        rdd.show()
    }
}
