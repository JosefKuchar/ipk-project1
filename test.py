#!/usr/bin/env python3
"""Tests for ipkcpc"""

import unittest
import subprocess
import argparse
import sys

ADDR = '127.0.0.1'
TCP_PORT = '1234'
UDP_PORT = '1235'


def run(args):
    """Run ipkcpc with supplied arguments and input"""
    pipe = subprocess.Popen(["./ipkcpc"] + args, shell=True,
                            stdin=subprocess.PIPE,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE)
    # Wait for exit code
    out = pipe.wait()
    # Close all streams
    pipe.stdout.close()
    pipe.stdin.close()
    pipe.stderr.close()
    # Return exit code and output
    return out


class TestArgumentParsing(unittest.TestCase):
    """Argument parsing tests"""

    def test_no_arguments(self):
        """No arguments"""
        self.assertNotEqual(run([]), 0)

    def test_invalid_mode(self):
        """Invalid mode"""
        self.assertNotEqual(run(['-h', ADDR, '-p', TCP_PORT, '-m', 'a']), 0)

    def test_invalid_host(self):
        """Invalid host"""
        self.assertNotEqual(
            run(['-h', 'foobar', '-p', TCP_PORT, '-m', 'tcp']), 0)

    def test_invalid_port(self):
        """Invalid port"""
        self.assertNotEqual(run(['-h', ADDR, '-p', 'foo', '-m', 'tcp']), 0)

    def test_missing_mode(self):
        """Missing mode"""
        self.assertNotEqual(run(['-h', ADDR, '-p', TCP_PORT,]), 0)

    def test_missing_host(self):
        """Missing host"""
        self.assertNotEqual(run(['-p', TCP_PORT, '-m', 'tcp']), 0)

    def test_missing_port(self):
        """Missing port"""
        self.assertNotEqual(run(['-h', ADDR, '-m', 'tcp']), 0)


if __name__ == "__main__":
    # Setup argument parser
    parser = argparse.ArgumentParser(description="ipkcpc testing script")
    parser.add_argument('-a', help="Server address", required=True)
    parser.add_argument('-t', help="Server TCP port", required=True)
    parser.add_argument('-u', help="Server UDP port", required=True)
    # Parse known arguments
    parsed_args = parser.parse_known_args()
    ADDR = parsed_args[0].a
    TCP_PORT = parsed_args[0].t
    UDP_PORT = parsed_args[0].u
    # Pass rest args to unittest
    sys.argv[1:] = parsed_args[1]
    # Run tests
    unittest.main()
