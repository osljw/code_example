import org.apache.spark.{ SparkConf, SparkContext }
import org.apache.spark.sql.SparkSession
import org.apache.spark.sql.{DataFrame, Row}
import org.apache.spark.sql.functions.{col, row_number, concat_ws}
import org.apache.spark.sql.types.{IntegerType, LongType, StringType, StructType}
import org.apache.spark.sql.expressions.Window

import scala.util.Try

object Spark {
    val conf: SparkConf = new SparkConf()
        .setAppName("SparkApp")
    val spark = SparkSession.builder()
        .config(conf)
        .enableHiveSupport()
        .getOrCreate()
    val sc: SparkContext = spark.sparkContext
}

import Spark._
import spark.implicits._

object SparkApp {
    def main(args: Array[String]) {

        val showSql = "show databases"
        val rdd = spark.sql(showSql)
        rdd.show()
        
        val showSql1 = "select * from test where dt='2019-08-29' and h=12 limit 1"
        val rdd1 = spark.sql(showSql1)
        rdd1.show()
    }
}
