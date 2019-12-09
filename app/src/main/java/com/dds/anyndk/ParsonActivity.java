package com.dds.anyndk;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.dds.parson.Parson;
import com.dds.parson.User;

public class ParsonActivity extends AppCompatActivity {

    private TextView text;
    private TextView text1;
    private String pro = "姓名:大大帅，\n职业：程序员，\n喜好：看书、打球、美女";

    public static void openActivity(AppCompatActivity activity) {
        Intent intent = new Intent(activity, ParsonActivity.class);
        activity.startActivity(intent);

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_parson);
        text = findViewById(R.id.text);
        text1 = findViewById(R.id.text1);
        text.setText(pro);
    }

    public void parseJson(View view) {
        String test = Parson.test();
        Log.d("dds_test", test);
        text1.setText(test);
    }

    public void decodeJson(View view) {
        String result = Parson.test();
        Log.d("dds_test", result);
        User user = Parson.parseJson(result);
        if (user != null) {
            try {
                Log.d("dds_test", "habit:" + user.getHabbits().get(0));
            } catch (Exception e) {
                e.printStackTrace();

            }
        }
    }
}
