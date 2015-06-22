<?php
/**
 * @file tpenconnector.inc
 * This file is where non Drupal hooks code live
 */

require "TPEN.inc";
require "TPENProjectContentType.inc";

function tpenconnector_get_manifest($object_pid = '') {
    
    $manifest = islandora_datastream_load("SC", $object_pid);
    
    //drupal_add_http_header('Content-Type', 'application/json; utf-8');
    //echo $manifest->content;
    //echo rawurlencode($manifest->content);

    $session_id = TPEN::login();

    if ($session_id) {
        // first, check if a project already exists in drupal for this user/ms 
        $uri = TPENProjectContentType::check_existing_project($object_pid);
        echo $uri;
        if (!$uri) {
            $uri = TPEN::create_project($manifest);
            //$uri = "http://tpendev:8080/tpen/project/4363";

            // add uri to drupal for this user
            TPENProjectContentType::add_tpen_project_node($object_pid, $uri);
        }

        // add /transcribe to URI for transcription interface URL
        header("Location: " . $uri . "/transcribe?session_id=" . $session_id);
    }else {
        echo "Error logging into T-PEN!";
    }
   
   
   /*
   $ch = curl_init();
   curl_setopt($ch, CURLOPT_URL, TPEN_URL . "/createProject");
   curl_setopt($ch, CURLOPT_POST, 1);
   curl_setopt($ch, CURLOPT_POSTFIELDS, $manifest->content);
   curl_exec($ch);
   curl_close($ch);   
   */
}






