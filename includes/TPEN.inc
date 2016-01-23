<?php

class TPEN {

    private static function do_curl($url, $method = 'GET', $data = array()) {
      $curl = curl_init();

      curl_setopt_array($curl, array(
        CURLOPT_URL => $url,
        CURLOPT_RETURNTRANSFER => true,
        CURLOPT_ENCODING => "",
        CURLOPT_MAXREDIRS => 10,
        CURLOPT_TIMEOUT => 0,
        CURLOPT_HTTP_VERSION => CURL_HTTP_VERSION_1_1,
        CURLOPT_CUSTOMREQUEST => $method
      ));

      if (!empty($data)) {
        curl_setopt($curl, CURLOPT_POSTFIELDS, $data)
      }

      $response = curl_exec($curl);
      $err = curl_error($curl);
      $code = curl_getinfo($curl, CURLINFO_HTTP_CODE);

      curl_close($curl);

      if ($err || $code > 400) {
          error_log("cURL received status $code and threw error #$err when attempting to load $url");
          return false;
      }

      return $response;
    }

    public static function create_project($manifest, $title, $user) {
      return TPEN::do_curl(variable_get('tpenconnector_tpen_url') . "/createProject?title=$title&user=$user",
                            'POST', 'scmanifest=' . rawurlencode($manifest->content));
    }

    public static function copy_project($project_id, $user) {
      return TPEN::do_curl(variable_get('tpenconnector_tpen_url') . "/copyProjectWithAnnotations?projectID=$project_id&user=$user");
    }

    public static function delete_project($project_id, $user) {
      return TPEN::do_curl(variable_get('tpenconnector_tpen_url') . "/delProject?projectID=$project_id&user=$user");
    }

}
