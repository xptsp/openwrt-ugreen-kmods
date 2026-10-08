'use strict';
'require form';
'require fs';
'require uci';
'require tools.widgets as widgets';

return L.view.extend({
	// Pull current raw disks present from /proc/diskstats to help populate choices
	load: function() {
		return fs.lines('/proc/diskstats').then(function(lines) {
			var disks = [];
			for (var i = 0; i < lines.length; i++) {
				var fields = lines[i].trim().split(/\s+/);
				if (fields.length >= 3) {
					var name = fields[2];
					// Simple filter targeting common drives (sda, sdb, nvme0n1, etc)
					if (name.match(/^(sd[a-z]|nvme[0-9]n[0-9])$/)) {
						disks.push(name);
					}
				}
			}
			return disks;
		}).catch(function() {
			return ['sda', 'sdb', 'sdc']; // Fallbacks if read permission drops
		});
	},

	render: function(disks) {
		var m, s, o;

		// Map configuration directly onto the /etc/config/hdd_led setup file
		m = new form.Map('hdd_led', _('HDD LED Monitor Configuration'),
			_('Configure background instances to monitor disk writes/reads and dynamically toggle LED hardware states based on S.M.A.R.T data patterns.'));

		// Define a multi-instance section mapping each distinct drive block
		s = m.section(form.GridSection, 'drive', _('Drive Bays / Monitor Instances'));
		s.anonymous = false;
		s.addremove = true; // Allows adding and deleting bays via the UI

		// Enable Toggle
		o = s.option(form.Flag, 'enabled', _('Enabled'));
		o.rmempty = false;
		o.default = '1';

		// Monitored Device Selection List
		o = s.option(form.ListValue, 'device', _('Target Drive'));
		for (var i = 0; i < disks.length; i++) {
			o.value(disks[i]);
		}
		o.rmempty = false;

		// Absolute file tree descriptor path node targeting ugreen drivers
		o = s.option(form.Value, 'led_file', _('Sysfs LED Target Node'));
		o.placeholder = '/sys/class/leds/ugreen_disk1/color';
		o.datatype = 'string';
		o.rmempty = false;

		// Execution poll pacing window
		o = s.option(form.Value, 'interval', _('Poll Interval (ms)'));
		o.placeholder = '100';
		o.datatype = 'uinteger';
		o.default = '100';
		o.rmempty = false;

		// Idle Color HEX Target
		o = s.option(form.Value, 'idle_color', _('Idle Color (Hex)'));
		o.placeholder = '#000028';
		o.datatype = 'string';
		o.validate = function(section_id, value) {
			if (!value.match(/^#(?:[0-9a-fA-F]{3}){1,2}$/) && !value.match(/^0x[0-9a-fA-F]{6}$/)) {
				return _('Must be valid hex format (e.g. #000028 or 0x000028)');
			}
			return true;
		};
		o.rmempty = false;

		// Flash Color HEX Target
		o = s.option(form.Value, 'act_color', _('Activity Flash (Hex)'));
		o.placeholder = '#00FF00';
		o.datatype = 'string';
		o.validate = function(section_id, value) {
			if (!value.match(/^#(?:[0-9a-fA-F]{3}){1,2}$/) && !value.match(/^0x[0-9a-fA-F]{6}$/)) {
				return _('Must be valid hex format (e.g. #00FF00)');
			}
			return true;
		};
		o.rmempty = false;

		// S.M.A.R.T Defect State Target Color
		o = s.option(form.Value, 'fault_color', _('SMART Fault Color (Hex)'));
		o.placeholder = '#FF0000';
		o.datatype = 'string';
		o.validate = function(section_id, value) {
			if (!value.match(/^#(?:[0-9a-fA-F]{3}){1,2}$/) && !value.match(/^0x[0-9a-fA-F]{6}$/)) {
				return _('Must be valid hex format (e.g. #FF0000)');
			}
			return true;
		};
		o.rmempty = false;

		// SMART Check Loop Interval Boundary Window
		o = s.option(form.Value, 'smart_interval', _('SMART Check Window (sec)'));
		o.placeholder = '600';
		o.datatype = 'uinteger';
		o.default = '600';
		o.rmempty = false;

		return m.render();
	}
});
