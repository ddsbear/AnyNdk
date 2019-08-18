package com.dds.anyndk;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

public class GmSSLActivity extends AppCompatActivity {

    private EditText editContent;
    private EditText editPwd;
    private TextView text;

    public static void openActivity(AppCompatActivity activity) {
        Intent intent = new Intent(activity, GmSSLActivity.class);
        activity.startActivity(intent);

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_gm_ssl);
        editContent = findViewById(R.id.edit_content);
        editPwd = findViewById(R.id.edit_pwd);
        text = findViewById(R.id.text);
    }

    public void generateRandom(View view) {

    }
}
