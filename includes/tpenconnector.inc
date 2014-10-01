<?php
/**
 * @file tpenconnector.inc
 * This file is where non Drupal hooks code live
 */

function tpenconnector_get_manifest($object_pid = '') {
   $object = islandora_object_load($object_pid);
   $api_a = $object->repository->api->a;

   global $base_url;

   $manifest = new SharedCanvasManifest($object->label, "{$base_url}/islandora/object/{$object_pid}/datastream/MODS");
   module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  /**
   * Each page is an array with following attributes.
   * pid - the pid of the page object
   * page - the page number? not sure
   * lagel - the label of the page
   * width - width
   * height - height
   */
  $pages = islandora_paged_content_get_pages($object);
  foreach ($pages as $page) {
    $page_object = islandora_object_load($page['pid']);
    $page_datastreams = $api_a->listDatastreams($page['pid']);
    $page_object_url = "{$base_url}/islandora/object/{$page['pid']}";
    $canvas = new Canvas($page['label'], $page_object_url."/datastream/MODS/view");
    $canvas->setImage($page_object_url."/datastream/OBJ/view", $page_datastreams['OBJ']['mimetype'], $page['width'], $page['height']);
    $manifest->addCanvas($canvas);
  }

  echo $manifest->getJson();
}