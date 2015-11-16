package download_data;
import java.io.*;
import java.util.*;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;

public class download_data {
	public static void main(String[] args){
		
		String url = "http://bigimbaz.inrialpes.fr/herve/siftgeo1M/";
		String path = "C:/Users/Subhankari/ML_data";
		int randomval[] = new int[1000];
		int randomval1[] = new int[1000];
		Random rn = new Random();
		for(int i = 0; i < 1000; i++){
			int val = rn.nextInt(1000);
			randomval[i] = val;
			System.out.println(randomval[i]);
		}
		
		for(int j = 0; j < 1000; j++){
			int val = rn.nextInt(999);
			randomval1[j] = val;
			System.out.println(randomval1[j]);
		}
		Arrays.sort(randomval);
		//Arrays.sort(randomval1);
				
		for(int i = 55; i < 1000; i++){
			int value = randomval[i];
			String folderval =  String.format("%04d", value );
			String folder = url + folderval + '/';
			String pathfd = path + folderval + '/';
			System.out.println(folder);
			for(int j = 0;j < 10; j++){
				int value2 = randomval1[j];
				String fileval =  String.format("%03d", value2);
				System.out.println(fileval);
				String file = folder + fileval + ".siftgeo";
				String pathfl = pathfd + fileval + ".siftgeo";
				
				String whattorun = "wget -r " + pathfl +" " + file;
				try
				   {
				     Runtime rt = Runtime.getRuntime();
				     Process proc = rt.exec(whattorun);
				     int exitVal = proc.waitFor();
				     System.out.println("Process exitValue:" + exitVal);
				   } catch (Throwable t)
				     {
				       t.printStackTrace();
				       break;
				     }
				
			}
		}
		
	}

}
