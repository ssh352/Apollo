<!DOCTYPE HTML>
<html>
<head>
<meta http-equiv="refresh" content="300">
<title>LLT覆盖率</title>
</head>
<body>
<?php
$con = mysql_connect("10.162.191.147","root","password");
if(!$con)
{
  die('Could not connect: ' . mysql_error());
}

mysql_select_db("CID_DA", $con);
$result = mysql_query("SELECT MAX(commit_time),project,code_lines,total_test_case,fail_case,spend_time,coverage FROM mydb_testcaseinfo_v2 WHERE spend_time != '' GROUP BY project");

echo "<table border='1' align='center'>
<h3 align='center'>LLT项目级覆盖率数据</h3>
<tr>
<th>项目名</th>
<th>总代码行</th>
<th>总用例数</th>
<th>失败用例数</th>
<th>执行时间(秒)</th>
<th>覆盖率(%)</th>
</tr>";

while($row = mysql_fetch_array($result))
  {
  echo "<tr>";
  echo "<td>" . $row['project'] . "</td>";
  echo "<td>" . $row['code_lines'] . "</td>";
  echo "<td>" . $row['total_test_case'] . "</td>";
  echo "<td>" . $row['fail_case'] . "</td>";
  echo "<td>" . $row['spend_time'] . "</td>";
  echo "<td>" . $row['coverage'] . "</td>";
  echo "</tr>";
  }
echo "</table>";

mysql_close($con);
?>
</body>
</html>
