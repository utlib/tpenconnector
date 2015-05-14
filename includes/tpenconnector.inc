<?php
/**
 * @file tpenconnector.inc
 * This file is where non Drupal hooks code live
 */

define(TPEN_URL, "http://zhaos.library.utoronto.ca:8080/newberry/");

function tpenconnector_get_manifest($object_pid = '') {
   $manifest = islandora_datastream_load("SC", $object_pid);
   //echo $manifest->content;
   
   $ch = curl_init();
   curl_setopt($ch, CURLOPT_URL, TPEN_URL);
   curl_setopt($ch, CURLOPT_POST, 1);
   curl_setopt($ch, CURLOPT_POSTFIELDS, $manifest->content);
   curl_exec($ch);
   curl_close($ch);   
}
