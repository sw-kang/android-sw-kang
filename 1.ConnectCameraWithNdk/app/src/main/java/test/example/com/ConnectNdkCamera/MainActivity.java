package test.example.com.ConnectNdkCamera;

import android.Manifest;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.Camera;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    ImageView m_LiveView = null;
    Button m_btn_start = null;
    Button m_btn_stop = null;
    Camera camera;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        m_LiveView = findViewById(R.id.imageView_liveView);
        m_btn_start = findViewById(R.id.button_start);
        m_btn_stop = findViewById(R.id.button_stop);

        int permissionCheck = ContextCompat.checkSelfPermission(this, Manifest.permission.CAMERA);

        if (permissionCheck == PackageManager.PERMISSION_DENIED) {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.CAMERA},100);
        } else {
            openCamera();
        }


        m_btn_start.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                camera.addCallbackBuffer(new byte[640*480*2]);
                camera.setPreviewCallbackWithBuffer(new Camera.PreviewCallback() {
                    @Override
                    public void onPreviewFrame(byte[] bytes, Camera camera) {

                        int[] rgb =  decodeYUV420SPndk(bytes,640,480);
                        Bitmap bmp = Bitmap.createBitmap( rgb,480, 640,Bitmap.Config.ARGB_8888);
                        m_LiveView.setImageBitmap(bmp);
                        camera.addCallbackBuffer(bytes);
                    }
                });
                camera.startPreview();;
            }
        });
        m_btn_stop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                camera.stopPreview();
            }
        });
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native int[] decodeYUV420SPndk(byte[] image , int width, int height);


    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    /**
     *
     */
    public void openCamera(){
        camera = Camera.open(Camera.CameraInfo.CAMERA_FACING_BACK);
        camera.addCallbackBuffer(new byte[640*480*2]);
        Camera.Parameters parameters = camera.getParameters();
        parameters.setPreviewSize(640,480);
        camera.setParameters(parameters);

        camera.setPreviewCallbackWithBuffer(new Camera.PreviewCallback() {
            @Override
            public void onPreviewFrame(byte[] bytes, Camera camera) {

                int[] rgb =  decodeYUV420SPndk(bytes,640,480);
                Bitmap bmp = Bitmap.createBitmap( rgb,480, 640,Bitmap.Config.ARGB_8888);
                m_LiveView.setImageBitmap(bmp);
                camera.addCallbackBuffer(bytes);
            }
        });
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if(grantResults[0]== PackageManager.PERMISSION_GRANTED){
            openCamera();
        }
    }
}
