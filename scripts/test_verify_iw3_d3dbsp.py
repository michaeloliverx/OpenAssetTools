"""Regression for the primary-light records required by the Radiant BSP loader."""
import struct
import tempfile
import unittest
from pathlib import Path

from verify_iw3_d3dbsp import validate


class PrimaryLightTests(unittest.TestCase):
    def check(self, count, first_type=0):
        lights = bytearray(128 * count)
        if count:
            lights[0] = first_type
        if count > 1:
            lights[128] = 1
        data = struct.pack('<4sII', b'IBSP', 22, 1) + struct.pack('<II', 43, len(lights)) + lights
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'test.d3dbsp'
            path.write_bytes(data)
            return validate(path)

    def test_missing_and_single_primary_lights_rejected(self):
        for count in (0, 1):
            with self.subTest(count=count), self.assertRaisesRegex(AssertionError, 'at least two primary lights'):
                self.check(count)

    def test_reserved_null_and_sun_accepted(self):
        self.assertEqual(self.check(2)['primary_lights'], 2)

    def test_non_null_first_slot_rejected(self):
        with self.assertRaisesRegex(AssertionError, 'slot zero must be null'):
            self.check(2, first_type=1)


if __name__ == '__main__':
    unittest.main()
