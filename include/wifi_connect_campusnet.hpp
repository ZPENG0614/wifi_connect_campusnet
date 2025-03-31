#ifndef broadnet_connect_hpp
#define broadnet_connect_hpp
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define wifi_name "CQUPT"
String URL = "http://192.168.200.2:801/eportal/?c=Portal&a=login&callback=dr1003&login_method=1"; // 校园网认证页面
String My_account = "1694096@cmcc";                                                               // 账号
String My_password = "luo5813177";                                                                // 密码
String My_ip;//ip由程序自动填充
String address_last = "&wlan_user_ipv6=&wlan_user_mac=145afc035099&wlan_ac_ip=192.168.200.1&wlan_ac_name=NE40E-X16A&jsVersion=3.3.3&v=8437";//后面的一坨东西

unsigned long lastCheckTime = 0; // 上次检查时间
const unsigned long checkInterval = 10*60000; // 检查间隔,10后面的单位是分钟

int wifi_connect()          //WIFI连接
{

  //连接WiFi
  WiFi.mode(WIFI_STA);        //设置STA模式
  Serial.println();           //换行
  Serial.printf("Mode=%d\r\n",WiFi.getMode());//读取本机模式
  WiFi.begin(wifi_name);        //初始化WIFI。ssid：ID，password：密码。校园网不需要密码，连接后进行认证即可
  Serial.println("WIFI Connecting.....");
  while(WiFi.status()!= WL_CONNECTED)        //等待连接完成
  {
    WiFi.begin(wifi_name);        //重新连接
    delay(2000);
  }
  Serial.println("WIFI Connected!");          //打印连接完成



  //进行认证
  Serial.print("IP Address:");                //获取IP地址
  Serial.println(WiFi.localIP());             //打印IP地址
 
  IPAddress localip = WiFi.localIP();
  My_ip = String(localip[0])+"."+String(localip[1])+"."+String(localip[2])+"."+String(localip[3]);
  HTTPClient http;        //创建HTTPClient类
  URL = URL+"&user_account=,0,"+My_account+"&user_password="+My_password+"&wlan_user_ip="+My_ip+address_last;//获取get请求的url
  Serial.println(URL);
  //发送GET请求
  http.begin(URL);
  int httpCode = http.GET();
  Serial.printf("HTTP状态码:%d\n",httpCode);//获取状态码
  //获取响应正文
  String response = http.getString();
  Serial.println("响应数据:");
  Serial.println(response);
  http.end();
  if(httpCode==HTTP_CODE_OK){
    Serial.println("认证成功");
  }
  else{
    Serial.println("认证失败");
  }
  return httpCode;
}


// 检查连接状态并重新连接
void wifi_check(){
  // 定时检查连接状态
  unsigned long currentTime = millis();
  if (currentTime - lastCheckTime >= checkInterval)
  {
    lastCheckTime = currentTime;
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("WiFi连接已断开，尝试重新连接...");
      wifi_connect(); // 递归调用自身进行重新连接和认证
    }
  }
}


#endif