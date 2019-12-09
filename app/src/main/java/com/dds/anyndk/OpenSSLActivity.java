package com.dds.anyndk;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.dds.openssl.OpenCipher;

public class OpenSSLActivity extends AppCompatActivity {

    private TextView text;
    private StringBuilder sb;

    public static void openActivity(AppCompatActivity activity) {
        Intent intent = new Intent(activity, OpenSSLActivity.class);
        activity.startActivity(intent);

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_open_ssl);
        text = findViewById(R.id.text);
    }

    public void generateRandom(View view) {

    }

    // 测试aes
    public void AES(View view) {
        sb = new StringBuilder();
        String content = "123456";
        String key = "GAOQXQQ99QPKOMTZE9YF96OLTD8EU6T9";
        sb.append("content:").append(content).append("\n");
        sb.append("key:").append(key).append("\n");

        String enc = OpenCipher.getInstance().AesEncode(content, key);
        sb.append("encode:").append(enc).append("\n");


        String dec = OpenCipher.getInstance().AesDecode(enc, key);
        sb.append("decode:").append(dec).append("\n");

        text.setText(sb.toString());
    }
}
