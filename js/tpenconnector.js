
(function ($) {
	$(document).ready(function() {
		$('.new-tpen-proj').click(function(event) {
			
			var transcribe_url = $(this).attr('id');

			$.ajax({
				url: "/"+transcribe_url,
				success: function(data) {
					alert(data);
					location.reload();
				}
			});

			
		});
	});
}(jQuery));