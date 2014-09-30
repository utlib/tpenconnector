<?php
class Canvas {
  protected $object_label;
  protected $metadata_uri;
  protected $image_uri;
  protected $image_format;
  protected $image_height;
  protected $image_width;

  function __construct($label, $meta_uri) {
    $this->object_label = $label;
    $this->metadata_uri = $meta_uri;
  }

  function setImage($uri, $format = 'image/jpeg', $height, $width) {
    $this->image_uri = $uri;
    $this->image_format = $format;
    $this->image_height = $height;
    $this->image_width = $width;
  }

  function toArray() {
    $returnArray = array (
        '@type' => 'sc:Canvas',
        'label' => $this->object_label,
        'metadata' => $this->metadata_uri,
        'images' => array ( // always contain only one
            array (
                '@type' => 'oa:Annotation',
                'motivation' => 'sc:Painting',
                'resource' => array (
                    '@id' => $this->image_uri,
                    '@type' => 'dctypes:Image',
                    'format' => $this->image_format,
                    'height' => $this->image_height,
                    'width' => $this->image_width
                )
            )
        )
    );

    return $returnArray;
  }
}