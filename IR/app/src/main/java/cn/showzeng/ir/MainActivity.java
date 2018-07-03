package cn.showzeng.ir;

import android.annotation.SuppressLint;
import android.content.Context;
import android.hardware.ConsumerIrManager;
import android.os.Build;
import android.support.annotation.RequiresApi;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import java.util.List;

import butterknife.BindViews;
import butterknife.ButterKnife;

@RequiresApi(api = Build.VERSION_CODES.KITKAT)

public class MainActivity extends AppCompatActivity implements View.OnClickListener{

    @BindViews({R.id.btn_led1, R.id.btn_led2, R.id.btn_led3, R.id.btn_led4, R.id.btn_led5, R.id.btn_led6})
    List<Button> leds;

    private ConsumerIrManager consumerIrManager;

    // NEC 协议固定引导码 9000,4500 以及结束码 560,20000
    // 用户码 2333 ==> 0010 0011 0011 0011 =(逆序)=> 1100 0100 1100 1100

    // 数据码 01 ==> 0000 0001 =(逆序)=> 1000 0000
    private int[] pattern1 = {
            9000,4500,
            560,1690, 560,1690, 560,560, 560,560, 560,560, 560,1690, 560,560, 560,560,
            560,1690, 560,1690, 560,560, 560,560, 560,1690, 560,1690, 560,560, 560,560,
            560,1690, 560,560, 560,560, 560,560, 560,560, 560,560, 560,560, 560,560,
            560,560, 560,1690, 560,1690, 560,1690, 560,1690, 560,1690, 560,1690, 560,1690,
            560,20000
    };

    // 数据码 02 ==> 0000 0010 =(逆序)=> 0100 0000
    private int[] pattern2 = {
            9000,4500,
            560,1690, 560,1690, 560,560, 560,560, 560,560, 560,1690, 560,560, 560,560,
            560,1690, 560,1690, 560,560, 560,560, 560,1690, 560,1690, 560,560, 560,560,
            560,560, 560,1690, 560,560, 560,560, 560,560, 560,560, 560,560, 560,560,
            560,1690, 560,560, 560,1690, 560,1690, 560,1690, 560,1690, 560,1690, 560,1690,
            560,20000
    };

    // 数据码 03 ==> 0000 0011 =(逆序)=> 1100 0000
    private int[] pattern3 = {
            9000,4500,
            560,1690, 560,1690, 560,560, 560,560, 560,560, 560,1690, 560,560, 560,560,
            560,1690, 560,1690, 560,560, 560,560, 560,1690, 560,1690, 560,560, 560,560,
            560,1690, 560,1690, 560,560, 560,560, 560,560, 560,560, 560,560, 560,560,
            560,560, 560,560, 560,1690, 560,1690, 560,1690, 560,1690, 560,1690, 560,1690,
            560,20000
    };

    // 数据码 04 ==> 0000 0100 =(逆序)=> 0010 0000
    private int[] pattern4 = {
            9000,4500,
            560,1690, 560,1690, 560,560, 560,560, 560,560, 560,1690, 560,560, 560,560,
            560,1690, 560,1690, 560,560, 560,560, 560,1690, 560,1690, 560,560, 560,560,
            560,560, 560,560, 560,1690, 560,560, 560,560, 560,560, 560,560, 560,560,
            560,1690, 560,1690, 560,560, 560,1690, 560,1690, 560,1690, 560,1690, 560,1690,
            560,20000
    };

    // 数据码 05 ==> 0000 0101 =(逆序)=> 1010 0000
    private int[] pattern5 = {
            9000,4500,
            560,1690, 560,1690, 560,560, 560,560, 560,560, 560,1690, 560,560, 560,560,
            560,1690, 560,1690, 560,560, 560,560, 560,1690, 560,1690, 560,560, 560,560,
            560,1690, 560,560, 560,1690, 560,560, 560,560, 560,560, 560,560, 560,560,
            560,560, 560,1690, 560,560, 560,1690, 560,1690, 560,1690, 560,1690, 560,1690,
            560,20000
    };

    // 数据码 06 ==> 0000 0110 =(逆序)=> 0110 0000
    private int[] pattern6 = {
            9000,4500,
            560,1690, 560,1690, 560,560, 560,560, 560,560, 560,1690, 560,560, 560,560,
            560,1690, 560,1690, 560,560, 560,560, 560,1690, 560,1690, 560,560, 560,560,
            560,560, 560,1690, 560,1690, 560,560, 560,560, 560,560, 560,560, 560,560,
            560,1690, 560,560, 560,560, 560,1690, 560,1690, 560,1690, 560,1690, 560,1690,
            560,20000
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ButterKnife.bind(this);

        initInfrared();
        for (Button led: leds) {
            led.setOnClickListener(this);
        }
    }

    @SuppressLint("DefaultLocale")
    private void initInfrared() {
        consumerIrManager = (ConsumerIrManager) getSystemService(Context.CONSUMER_IR_SERVICE);
        assert consumerIrManager != null;
        if (!consumerIrManager.hasIrEmitter()) {
            Toast.makeText(this, "当前手机不支持红外遥控", Toast.LENGTH_SHORT).show();
        } else {
            Toast.makeText(this, "红外设备就绪", Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.btn_led1:
                sendMessage(pattern1);
                break;
            case R.id.btn_led2:
                sendMessage(pattern2);
                break;
            case R.id.btn_led3:
                sendMessage(pattern3);
                break;
            case R.id.btn_led4:
                sendMessage(pattern4);
                break;
            case R.id.btn_led5:
                sendMessage(pattern5);
                break;
            case R.id.btn_led6:
                sendMessage(pattern6);
                break;
        }
    }

    private void sendMessage(int[] pattern) {
        consumerIrManager.transmit(38000, pattern);
        Toast.makeText(this, "发射完成", Toast.LENGTH_SHORT).show();
    }
}
