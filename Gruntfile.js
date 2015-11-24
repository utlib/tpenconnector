module.exports = function(grunt) {
	'use strict';

	require('load-grunt-tasks')(grunt);
	
	var debug = !!grunt.option('debug');

	// Project configuration.
	grunt.initConfig({
		pkg: grunt.file.readJSON('package.json'),
		watch: {
			sass: {
				files: ['**/*.scss'],
				tasks: ['compass:dev', 'drush:cc_all']
			},
			js: {
				files: ['js/*.js'],
				tasks: ['drush:cc_all']
			}
		},
		drush: {
			cc_all: {
				args: ['cc' ,'all']
			}
		},

		compass: {
			dev: {
				options: {
					config: 'config.rb'
/*					basePath: ".",
					sassDir: 'sass',
					cssDir: 'css',
					outputStyle: 'compressed',
					noLineComments: 'false',
					relativeAssets: false,*/

				}
			}
		}

	});
	grunt.loadNpmTasks('grunt-drush');
	grunt.loadNpmTasks('grunt-contrib-compass');
	grunt.loadNpmTasks('grunt-contrib-sass');
	grunt.loadNpmTasks('grunt-contrib-watch');
};
