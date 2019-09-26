import org.apache.spark.{ SparkConf, SparkContext }
import org.apache.spark.sql.SparkSession

object SparkApp {
    def main(args: Array[String]) {
        val spark = SparkSession.builder()
            .enableHiveSupport()
            .getOrCreate()
        //val hiveContext = spark.sparkContext

        val showSql = "show databases"
        val rdd = spark.sql(showSql)

        rdd.show()
    }
}
