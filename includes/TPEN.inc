<?php
define(TPEN_URL, "***REMOVED***");

class TPEN {

    public static function login() {
        $curl = curl_init();

        curl_setopt_array($curl, array(
            CURLOPT_PORT => "8080",
            CURLOPT_URL => TPEN_URL . "/login",
            CURLOPT_RETURNTRANSFER => true,
            CURLOPT_ENCODING => "",
            CURLOPT_MAXREDIRS => 10,
            CURLOPT_TIMEOUT => 30,
            CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1,
            CURLOPT_CUSTOMREQUEST => "POST",
            CURLOPT_POSTFIELDS => "uname=***REMOVED***&password=3",
        ));


        $response = curl_exec($curl);
        $err = curl_error($curl);

        curl_close($curl);
        if ($err) {
            echo "cURL Error #:" . $err;
            return false;
        } else {
            return $response;
        }
    }

    public static function create_project($manifest) {
        $curl = curl_init();

        curl_setopt_array($curl, array(
          CURLOPT_PORT => "8080",
          CURLOPT_URL => TPEN_URL . "/createProject?user=***REMOVED***",
          CURLOPT_RETURNTRANSFER => true,
          CURLOPT_ENCODING => "",
          CURLOPT_MAXREDIRS => 10,
          CURLOPT_TIMEOUT => 30,
          CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1,
          CURLOPT_CUSTOMREQUEST => "POST",
          CURLOPT_POSTFIELDS => "scmanifest=" . rawurlencode($manifest->content),
        ));

        $response = curl_exec($curl);
        $err = curl_error($curl);

        curl_close($curl);

        if ($err) {
            echo "cURL Error #:" . $err;
        } else {
            return $response;
        }
    }


}
