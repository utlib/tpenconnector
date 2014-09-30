<?php
/**
 * @file tpenconnector.inc
 * This file is where non Drupal hooks code live
 */

function tpenconnector_get_manifest($object_pid = '') {
//   $manifest = new SharedCanvasManifest('label of the manifest', 'url to the MODS');
//   $canvas = new Canvas('Page One', 'url to page one MODS');
//   $canvas->setImage('uri to page one image', 'image/jpeg', '100', '200');

//   $manifest->addCanvas($canvas);
//   $json_string = $manifest->get_json();
//   echo $json_string;

  $object = islandora_object_load($object_pid);
  $dc = $object->getDatastream('DC');
  echo '<pre>'.print_r($dc->location, TRUE).'</pre>';
  $manifest = new SharedCanvasManifest($object->label, 'url to MODS');
  echo $manifest->getJson();
}