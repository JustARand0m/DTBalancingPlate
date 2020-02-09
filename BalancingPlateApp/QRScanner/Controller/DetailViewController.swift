//
//  DetailViewController.swift
//  QRScanner
//

import UIKit
import CoreMotion
import SystemConfiguration

class DetailViewController: UIViewController {

    @IBOutlet weak var detailLabel: CopyLabel!
    @IBOutlet weak var X_Label: CopyLabel!
    @IBOutlet weak var Y_Label: CopyLabel!
    
    var qrData: QRData?
    var motion = CMMotionManager()
    var addr: sockaddr_in = sockaddr_in()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        detailLabel.text = qrData?.codeString
        var scannedIP = (qrData?.codeString)!
        scannedIP = scannedIP.trimmingCharacters(in: .whitespacesAndNewlines)
        
        addr.sin_len = UInt8(MemoryLayout.size(ofValue: addr))
        addr.sin_family = sa_family_t(AF_INET)
        inet_pton(AF_INET, scannedIP, &addr.sin_addr)
        
        let reachability = withUnsafePointer(to: &addr, {
            $0.withMemoryRebound(to: sockaddr.self, capacity: 1) {
                SCNetworkReachabilityCreateWithAddress(kCFAllocatorDefault, $0)
            }
        })
        
        startQueuedUpdates()

    }
    
    let INADDR_ANY = in_addr(s_addr: 0)

    func udpSend(textToSend: String, address: in_addr, port: CUnsignedShort) {


    func htons(value: CUnsignedShort) -> CUnsignedShort {
        return (value << 8) + (value >> 8);
    }

    let fd = socket(AF_INET, SOCK_DGRAM, 0) // DGRAM makes it UDP

    let addr = sockaddr_in(
        sin_len:    __uint8_t(MemoryLayout<sockaddr_in>.size),
        sin_family: sa_family_t(AF_INET),
        sin_port:   htons(value: port),
        sin_addr:   address,
        sin_zero:   ( 0, 0, 0, 0, 0, 0, 0, 0 )
    )

    let sent = textToSend.withCString { cstr -> Int in

        var localCopy = addr

        let sent = withUnsafePointer(to: &localCopy) { pointer -> Int in
            let memory = UnsafeRawPointer(pointer).bindMemory(to: sockaddr.self, capacity: 1)
            let sent = sendto(fd, cstr, strlen(cstr), 0, memory, socklen_t(addr.sin_len))
            return sent
        }

        return sent
    }

    close(fd)
    }
    
    func startQueuedUpdates() {
       if motion.isDeviceMotionAvailable {       self.motion.deviceMotionUpdateInterval = 1.0 / 60.0
          self.motion.showsDeviceMovementDisplay = true
          self.motion.startDeviceMotionUpdates(using: .xMagneticNorthZVertical, 
                                               to: OperationQueue.current!, withHandler: { (data, error) in
             // Make sure the data is valid before accessing it.
                                                let r = (Double(data!.attitude.roll * 180 / Double.pi).rounded(toPlaces :3))
                                                let p = (Double(data!.attitude.pitch * 180 / Double.pi).rounded(toPlaces :3))
                                                //let y = (Double(data!.attitude.yaw * 180 / Double.pi).rounded(toPlaces :3))
                                                //print("x: ", r, "\t\ty: ", p, "\t\tz: ", y)
             if let validData = data {
                // Get the attitude relative to the magnetic north reference frame. 
                let roll = Double(data!.attitude.roll * 180 / Double.pi).rounded(toPlaces :3)
                let pitch = Double(data!.attitude.pitch * 180 / Double.pi).rounded(toPlaces :3)
                let yaw = Double(data!.attitude.yaw * 180 / Double.pi).rounded(toPlaces :3)

                self.X_Label.text = "x: \(-roll*0.5)"
                self.Y_Label.text = "y: \(-pitch*0.5)"
                
                self.udpSend(textToSend: "\(-pitch*0.5);\(-roll*0.5);", address: self.addr.sin_addr, port: 1337)
                // Use the motion data in your app.
             }
          })
       }
    }
}

extension Double {
    /// Rounds the double to decimal places value
    func rounded(toPlaces places:Int) -> Double {
        let divisor = pow(10.0, Double(places))
        return (self * divisor).rounded() / divisor
    }
    
    
}
