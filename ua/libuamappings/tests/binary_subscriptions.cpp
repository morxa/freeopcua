/// @author Alexander Rykovanov 2012
/// @email rykovanov.as@gmail.com
/// @brief Opc Ua Binary. Subscription service.
/// @license GNU LGPL
///
/// Distributed under the GNU LGPL License
/// (See accompanying file LICENSE or copy at
/// http://www.gnu.org/licenses/lgpl.html)
///

#include "common.h"

#include <opc/ua/extension_identifiers.h>
#include <opc/ua/message_identifiers.h>
#include <opc/ua/protocol/binary/stream.h>
#include <opc/ua/protocol/types.h>
#include <opc/ua/protocol/subscriptions.h>

#include <algorithm>
#include <stdexcept>

class SubscriptionSerialization : public OpcUaBinarySerialization
{
};

class SubscriptionDeserialization : public OpcUaBinaryDeserialization
{
};


//-------------------------------------------------------
// SubScriptionParameters
//-------------------------------------------------------

TEST_F(SubscriptionSerialization, SubscriptionParameters)
{

  using namespace OpcUa;
  using namespace OpcUa::Binary;

  SubscriptionParameters params;
  params.RequestedPublishingInterval = 1200000;
  params.RequestedLifetimeCount = 2;
  params.RequestedMaxKeepAliveCount = 3;
  params.MaxNotificationsPerPublish = 4;
  params.PublishingEnabled = true;
  params.Priority = 5;

  GetStream() << params << flush;

  const std::vector<char> expectedData = {
    0, 0, 0, 0, (char)0x80, (char)0x4f, (char)0x32, (char)0x41,
    2,0,0,0,
    3,0,0,0,
    4,0,0,0,
    1,
    5
  };

  ASSERT_EQ(expectedData, GetChannel().SerializedData) << PrintData(GetChannel().SerializedData) << std::endl << PrintData(expectedData);
  ASSERT_EQ(expectedData.size(), RawSize(params));
}

TEST_F(SubscriptionDeserialization, SubscriptionParameters)
{
  using namespace OpcUa;
  using namespace OpcUa::Binary;

  const std::vector<char> expectedData = {
    0, 0, 0, 0, (char)0x80, (char)0x4f, (char)0x32, (char)0x41,
    2,0,0,0,
    3,0,0,0,
    4,0,0,0,
    1,
    5
  };

  GetChannel().SetData(expectedData);

  SubscriptionParameters params;
  GetStream() >> params;

  ASSERT_EQ(params.RequestedPublishingInterval, 1200000);
  ASSERT_EQ(params.RequestedLifetimeCount, 2);
  ASSERT_EQ(params.RequestedMaxKeepAliveCount, 3);
  ASSERT_EQ(params.MaxNotificationsPerPublish, 4);
  ASSERT_EQ(params.PublishingEnabled, true);
  ASSERT_EQ(params.Priority, 5);
}

//-------------------------------------------------------
// CreateSubscriptionRequest
//-------------------------------------------------------

TEST_F(SubscriptionSerialization, CreateSubscriptionRequest)
{
  using namespace OpcUa;
  using namespace OpcUa::Binary;

  CreateSubscriptionRequest request;

  ASSERT_EQ(request.TypeID.Encoding, EV_FOUR_BYTE);
  ASSERT_EQ(request.TypeID.FourByteData.NamespaceIndex, 0);
  ASSERT_EQ(request.TypeID.FourByteData.Identifier, OpcUa::CREATE_SUBSCRIPTION_REQUEST);

  FILL_TEST_REQUEST_HEADER(request.Header);

  request.Parameters.RequestedPublishingInterval = 1200000;
  request.Parameters.RequestedLifetimeCount = 2;
  request.Parameters.RequestedMaxKeepAliveCount = 3;
  request.Parameters.MaxNotificationsPerPublish = 4;
  request.Parameters.PublishingEnabled = true;
  request.Parameters.Priority = 5;

  GetStream() << request << flush;

  const std::vector<char> expectedData = {
    1, 0, (char)0x13, 0x3, // TypeID

    // RequestHeader
    TEST_REQUEST_HEADER_BINARY_DATA,

    0, 0, 0, 0, (char)0x80, (char)0x4f, (char)0x32, (char)0x41,
    2,0,0,0,
    3,0,0,0,
    4,0,0,0,
    1,
    5
  };

  ASSERT_EQ(expectedData, GetChannel().SerializedData) << PrintData(GetChannel().SerializedData) << std::endl << PrintData(expectedData);
  ASSERT_EQ(expectedData.size(), RawSize(request));
}

TEST_F(SubscriptionDeserialization, CreateSubscriptionRequest)
{
  using namespace OpcUa;
  using namespace OpcUa::Binary;

  const std::vector<char> expectedData = {
    1, 0, (char)0x13, 0x3, // TypeID

    // RequestHeader
    TEST_REQUEST_HEADER_BINARY_DATA,

    0, 0, 0, 0, (char)0x80, (char)0x4f, (char)0x32, (char)0x41,
    2,0,0,0,
    3,0,0,0,
    4,0,0,0,
    1,
    5
  };

  GetChannel().SetData(expectedData);

  CreateSubscriptionRequest request;
  GetStream() >> request;

  ASSERT_EQ(request.TypeID.Encoding, EV_FOUR_BYTE);
  ASSERT_EQ(request.TypeID.FourByteData.NamespaceIndex, 0);
  ASSERT_EQ(request.TypeID.FourByteData.Identifier, OpcUa::CREATE_SUBSCRIPTION_REQUEST);

  ASSERT_REQUEST_HEADER_EQ(request.Header);

  ASSERT_EQ(request.Parameters.RequestedPublishingInterval, 1200000);
  ASSERT_EQ(request.Parameters.RequestedLifetimeCount, 2);
  ASSERT_EQ(request.Parameters.RequestedMaxKeepAliveCount, 3);
  ASSERT_EQ(request.Parameters.MaxNotificationsPerPublish, 4);
  ASSERT_EQ(request.Parameters.PublishingEnabled, true);
  ASSERT_EQ(request.Parameters.Priority, 5);
}

//-------------------------------------------------------
// SubscriptionData
//-------------------------------------------------------

TEST_F(SubscriptionSerialization, SubscriptionData)
{

  using namespace OpcUa;
  using namespace OpcUa::Binary;

  SubscriptionData data;
  data.ID = 2;
  data.RevisedPublishingInterval = 1200000;
  data.RevisedLifetimeCount = 3;
  data.RevizedMaxKeepAliveCount = 4;

  GetStream() << data << flush;

  const std::vector<char> expectedData = {
    2,0,0,0,
    0, 0, 0, 0, (char)0x80, (char)0x4f, (char)0x32, (char)0x41,
    3,0,0,0,
    4,0,0,0,
  };

  ASSERT_EQ(expectedData, GetChannel().SerializedData) << PrintData(GetChannel().SerializedData) << std::endl << PrintData(expectedData);
  ASSERT_EQ(expectedData.size(), RawSize(data));
}

TEST_F(SubscriptionDeserialization, SubscriptionData)
{
  using namespace OpcUa;
  using namespace OpcUa::Binary;

  const std::vector<char> expectedData = {
    2,0,0,0,
    0, 0, 0, 0, (char)0x80, (char)0x4f, (char)0x32, (char)0x41,
    3,0,0,0,
    4,0,0,0,
  };

  GetChannel().SetData(expectedData);

  SubscriptionData data;
  GetStream() >> data;

  ASSERT_EQ(data.ID, 2);
  ASSERT_EQ(data.RevisedPublishingInterval, 1200000);
  ASSERT_EQ(data.RevisedLifetimeCount, 3);
  ASSERT_EQ(data.RevizedMaxKeepAliveCount, 4);
}
//-------------------------------------------------------
// CreateSubscriptionResponse
//-------------------------------------------------------

TEST_F(SubscriptionSerialization, CreateSubscriptionResponse)
{
  using namespace OpcUa;
  using namespace OpcUa::Binary;

  CreateSubscriptionResponse response;

  ASSERT_EQ(response.TypeID.Encoding, EV_FOUR_BYTE);
  ASSERT_EQ(response.TypeID.FourByteData.NamespaceIndex, 0);
  ASSERT_EQ(response.TypeID.FourByteData.Identifier, OpcUa::CREATE_SUBSCRIPTION_RESPONSE);

  FILL_TEST_RESPONSE_HEADER(response.Header);

  response.Data.ID = 2;
  response.Data.RevisedPublishingInterval = 1200000;
  response.Data.RevisedLifetimeCount = 3;
  response.Data.RevizedMaxKeepAliveCount = 4;

  GetStream() << response << flush;

  const std::vector<char> expectedData = {
    1, 0, (char)0x16, 0x3, // TypeID

    // RequestHeader
    TEST_RESPONSE_HEADER_BINARY_DATA,

    2,0,0,0,
    0, 0, 0, 0, (char)0x80, (char)0x4f, (char)0x32, (char)0x41,
    3,0,0,0,
    4,0,0,0,
  };

  ASSERT_EQ(expectedData, GetChannel().SerializedData) << PrintData(GetChannel().SerializedData) << std::endl << PrintData(expectedData);
  ASSERT_EQ(expectedData.size(), RawSize(response));
}

TEST_F(SubscriptionDeserialization, CreateSubscriptionResponse)
{
  using namespace OpcUa;
  using namespace OpcUa::Binary;

  const std::vector<char> expectedData = {
    1, 0, (char)0x16, 0x3, // TypeID

    // RequestHeader
    TEST_RESPONSE_HEADER_BINARY_DATA,

    2,0,0,0,
    0, 0, 0, 0, (char)0x80, (char)0x4f, (char)0x32, (char)0x41,
    3,0,0,0,
    4,0,0,0,
  };

  GetChannel().SetData(expectedData);

  CreateSubscriptionResponse response;
  GetStream() >> response;

  ASSERT_EQ(response.TypeID.Encoding, EV_FOUR_BYTE);
  ASSERT_EQ(response.TypeID.FourByteData.NamespaceIndex, 0);
  ASSERT_EQ(response.TypeID.FourByteData.Identifier, OpcUa::CREATE_SUBSCRIPTION_RESPONSE);

  ASSERT_RESPONSE_HEADER_EQ(response.Header);

  ASSERT_EQ(response.Data.ID, 2);
  ASSERT_EQ(response.Data.RevisedPublishingInterval, 1200000);
  ASSERT_EQ(response.Data.RevisedLifetimeCount, 3);
  ASSERT_EQ(response.Data.RevizedMaxKeepAliveCount, 4);
}

//-------------------------------------------------------
// SubscriptionAcknowledgement
//-------------------------------------------------------

TEST_F(SubscriptionSerialization, SubscriptionAcknowledgement)
{

  using namespace OpcUa;
  using namespace OpcUa::Binary;

  SubscriptionAcknowledgement ack;
  ack.SubscriptionID = 1;
  ack.SequenceNumber = 2;

  GetStream() << ack << flush;

  const std::vector<char> expectedData = {
    1,0,0,0,
    2,0,0,0,
  };

  ASSERT_EQ(expectedData, GetChannel().SerializedData) << PrintData(GetChannel().SerializedData) << std::endl << PrintData(expectedData);
  ASSERT_EQ(expectedData.size(), RawSize(ack));
}

TEST_F(SubscriptionDeserialization, SubscriptionAcknowledgement)
{
  using namespace OpcUa;
  using namespace OpcUa::Binary;

  const std::vector<char> expectedData = {
    1,0,0,0,
    2,0,0,0,
  };

  GetChannel().SetData(expectedData);

  SubscriptionAcknowledgement ack;
  GetStream() >> ack;

  ASSERT_EQ(ack.SubscriptionID, 1);
  ASSERT_EQ(ack.SequenceNumber, 2);
}

//-------------------------------------------------------
// PublishParameters
//-------------------------------------------------------

TEST_F(SubscriptionSerialization, PublishParameters)
{
  using namespace OpcUa;
  using namespace OpcUa::Binary;

  SubscriptionAcknowledgement ack;
  ack.SubscriptionID = 1;
  ack.SequenceNumber = 2;

  PublishParameters params;
  params.Acknowledgements.push_back(ack);

  GetStream() << params << flush;

  const std::vector<char> expectedData = {
    1,0,0,0, // Count of acks
    1,0,0,0,
    2,0,0,0,
  };

  ASSERT_EQ(expectedData, GetChannel().SerializedData) << PrintData(GetChannel().SerializedData) << std::endl << PrintData(expectedData);
  ASSERT_EQ(expectedData.size(), RawSize(params));
}

TEST_F(SubscriptionDeserialization, PublishParameters)
{
  using namespace OpcUa;
  using namespace OpcUa::Binary;

  const std::vector<char> expectedData = {
    1,0,0,0, // Count of acks
    1,0,0,0,
    2,0,0,0,
  };

  GetChannel().SetData(expectedData);

  PublishParameters params;
  GetStream() >> params;

  ASSERT_EQ(params.Acknowledgements.size(), 1);
}

//-------------------------------------------------------
// CreateSubscriptionRequest
//-------------------------------------------------------

TEST_F(SubscriptionSerialization, PublishRequest)
{
  using namespace OpcUa;
  using namespace OpcUa::Binary;

  PublishRequest request;

  ASSERT_EQ(request.TypeID.Encoding, EV_FOUR_BYTE);
  ASSERT_EQ(request.TypeID.FourByteData.NamespaceIndex, 0);
  ASSERT_EQ(request.TypeID.FourByteData.Identifier, OpcUa::PUBLISH_REQUEST);

  FILL_TEST_REQUEST_HEADER(request.Header);

  SubscriptionAcknowledgement ack;
  ack.SubscriptionID = 1;
  ack.SequenceNumber = 2;

  request.Parameters.Acknowledgements.push_back(ack);

  GetStream() << request << flush;

  const std::vector<char> expectedData = {
    1, 0, (char)0x3A, 0x3, // TypeID

    // RequestHeader
    TEST_REQUEST_HEADER_BINARY_DATA,

    // Parameters
    1,0,0,0, // Count of acks
    1,0,0,0,
    2,0,0,0
  };

  ASSERT_EQ(expectedData, GetChannel().SerializedData) << PrintData(GetChannel().SerializedData) << std::endl << PrintData(expectedData);
  ASSERT_EQ(expectedData.size(), RawSize(request));
}

TEST_F(SubscriptionDeserialization, PublishRequest)
{
  using namespace OpcUa;
  using namespace OpcUa::Binary;

  const std::vector<char> expectedData = {
    1, 0, (char)0x3A, 0x3, // TypeID

    // RequestHeader
    TEST_REQUEST_HEADER_BINARY_DATA,

    // Parameters
    1,0,0,0, // Count of acks
    1,0,0,0,
    2,0,0,0
  };

  GetChannel().SetData(expectedData);

  PublishRequest request;
  GetStream() >> request;

  ASSERT_EQ(request.TypeID.Encoding, EV_FOUR_BYTE);
  ASSERT_EQ(request.TypeID.FourByteData.NamespaceIndex, 0);
  ASSERT_EQ(request.TypeID.FourByteData.Identifier, OpcUa::PUBLISH_REQUEST);

  ASSERT_REQUEST_HEADER_EQ(request.Header);

  ASSERT_EQ(request.Parameters.Acknowledgements.size(), 1);
}

//-------------------------------------------------------
// NotificationMessage
//-------------------------------------------------------

TEST_F(SubscriptionSerialization, NotificationMessage)
{
  using namespace OpcUa;
  using namespace OpcUa::Binary;

  NotificationMessage data;
  data.SequenceID = 1;
  data.PublishTime = 2;
  data.Data.push_back(NotificationData());

  GetStream() << data << flush;

  const std::vector<char> expectedData = {
    1,0,0,0, // SequenceID
    2,0,0,0,0,0,0,0, // PublishTime

    1,0,0,0, // Count of data
    //Message.Header
    0,0, // TypeID
    0,   // Encoding
  };

  ASSERT_EQ(expectedData, GetChannel().SerializedData) << PrintData(GetChannel().SerializedData) << std::endl << PrintData(expectedData);
  ASSERT_EQ(expectedData.size(), RawSize(data));
}

TEST_F(SubscriptionDeserialization, NotificationMessage)
{
  using namespace OpcUa;
  using namespace OpcUa::Binary;

  const std::vector<char> expectedData = {
    1,0,0,0, // SequenceID
    2,0,0,0,0,0,0,0, // PublishTime
    // Data vector
    1,0,0,0, //Count of Data
    0,0, // Data.Header.TypeID
    0,   // Data.Header.Encoding
  };

  GetChannel().SetData(expectedData);

  NotificationMessage message;
  GetStream() >> message;

  ASSERT_EQ(message.SequenceID, IntegerID(1));
  ASSERT_EQ(message.PublishTime, 2);
}

//-------------------------------------------------------
// PublishResult
//-------------------------------------------------------

TEST_F(SubscriptionSerialization, PublishResult)
{
  using namespace OpcUa;
  using namespace OpcUa::Binary;

  PublishResult result;
  result.SubscriptionID = 1;
  result.AvailableSequenceNumber.push_back(2);
  result.MoreNotifications = true;

  result.Message.SequenceID = 1;
  result.Message.PublishTime = 2;
  result.Message.Data.push_back(NotificationData());

  result.Statuses.push_back(StatusCode::Good);

  DiagnosticInfo diag1;
  diag1.EncodingMask = static_cast<DiagnosticInfoMask>(DIM_LOCALIZED_TEXT | DIM_INNER_DIAGNOSTIC_INFO);
  diag1.LocalizedText = 4;
  DiagnosticInfo diag2;
  diag2.EncodingMask = DIM_ADDITIONAL_INFO;
  diag2.AdditionalInfo = "add";
  result.Diagnostics.push_back(diag1);
  result.Diagnostics.push_back(diag2);

  GetStream() << result << flush;

  const std::vector<char> expectedData = {
    1,0,0,0, // SubscriptionID
    //AvailableSequenceNumbers
    1,0,0,0, // count
    2,0,0,0,
    // MoreNotifications
    1,

    // NotificationData
    1,0,0,0, // SequenceID
    2,0,0,0,0,0,0,0, // PublishTime
    // Data vector
    1,0,0,0, //Count of Data
    0,0, // Data.Header.TypeID
    0,   // Data.Header.Encoding

    // Statuses
    1,0,0,0,
    0,0,0,0,
    // Diagnostics
    2,0,0,0, // Count
    static_cast<DiagnosticInfoMask>(DIM_LOCALIZED_TEXT | DIM_INNER_DIAGNOSTIC_INFO), 4,0,0,0, \
    DIM_ADDITIONAL_INFO, 3, 0, 0, 0, 'a', 'd', 'd', \
  };

  ASSERT_EQ(expectedData, GetChannel().SerializedData) << "Actual:" << std::endl << PrintData(GetChannel().SerializedData) << std::endl << "Expected" << std::endl << PrintData(expectedData);
  ASSERT_EQ(expectedData.size(), RawSize(result));
}

TEST_F(SubscriptionDeserialization, PublishResult)
{
  using namespace OpcUa;
  using namespace OpcUa::Binary;

  const std::vector<char> expectedData = {
    1,0,0,0, // SubscriptionID
    //AvailableSequenceNumbers
    1,0,0,0, // count
    2,0,0,0,
    // MoreNotifications
    1,

    // NotificationData
    1,0,0,0, // SequenceID
    2,0,0,0,0,0,0,0, // PublishTime
    // Data vector
    1,0,0,0, //Count of Data
    0,0, // Data.Header.TypeID
    0,   // Data.Header.Encoding

    // Statuses
    1,0,0,0,
    0,0,0,0,
    // Diagnostics
    2,0,0,0, // Count
    static_cast<DiagnosticInfoMask>(DIM_LOCALIZED_TEXT | DIM_INNER_DIAGNOSTIC_INFO), 4,0,0,0, \
    DIM_ADDITIONAL_INFO, 3, 0, 0, 0, 'a', 'd', 'd', \
  };

  GetChannel().SetData(expectedData);

  PublishResult result;
  GetStream() >> result;

  ASSERT_EQ(result.SubscriptionID, 1);
  ASSERT_EQ(result.AvailableSequenceNumber.size(), 1);
  ASSERT_EQ(result.MoreNotifications, true);
  ASSERT_EQ(result.Message.Data.size(), 1);
  ASSERT_EQ(result.Statuses.size(), 1);
  ASSERT_EQ(result.Diagnostics.size(), 2);
}

//-------------------------------------------------------
// PublishResponse
//-------------------------------------------------------

TEST_F(SubscriptionSerialization, PublishResponse)
{
  using namespace OpcUa;
  using namespace OpcUa::Binary;

  PublishResponse response;

  ASSERT_EQ(response.TypeID.Encoding, EV_FOUR_BYTE);
  ASSERT_EQ(response.TypeID.FourByteData.NamespaceIndex, 0);
  ASSERT_EQ(response.TypeID.FourByteData.Identifier, OpcUa::PUBLISH_RESPONSE);

  FILL_TEST_RESPONSE_HEADER(response.Header);

  PublishResult result;
  response.Result.SubscriptionID = 1;
  response.Result.AvailableSequenceNumber.push_back(2);
  response.Result.MoreNotifications = true;
  response.Result.Message.SequenceID = 1;
  response.Result.Message.PublishTime = 2;
  response.Result.Message.Data.push_back(NotificationData());
  response.Result.Statuses.push_back(StatusCode::Good);

  DiagnosticInfo diag1;
  diag1.EncodingMask = static_cast<DiagnosticInfoMask>(DIM_LOCALIZED_TEXT | DIM_INNER_DIAGNOSTIC_INFO);
  diag1.LocalizedText = 4;
  DiagnosticInfo diag2;
  diag2.EncodingMask = DIM_ADDITIONAL_INFO;
  diag2.AdditionalInfo = "add";
  response.Result.Diagnostics.push_back(diag1);
  response.Result.Diagnostics.push_back(diag2);

  GetStream() << response << flush;

  const std::vector<char> expectedData = {
    1, 0, (char)0x3D, 0x3, // TypeID

    // RequestHeader
    TEST_RESPONSE_HEADER_BINARY_DATA,

    1,0,0,0, // SubscriptionID
    //AvailableSequenceNumbers
    1,0,0,0, // count
    2,0,0,0,
    // MoreNotifications
    1,

    // NotificationData
    1,0,0,0, // SequenceID
    2,0,0,0,0,0,0,0, // PublishTime
    // Data vector
    1,0,0,0, //Count of Data
    0,0, // Data.Header.TypeID
    0,   // Data.Header.Encoding

    // Statuses
    1,0,0,0,
    0,0,0,0,
    // Diagnostics
    2,0,0,0, // Count
    static_cast<DiagnosticInfoMask>(DIM_LOCALIZED_TEXT | DIM_INNER_DIAGNOSTIC_INFO), 4,0,0,0, \
    DIM_ADDITIONAL_INFO, 3, 0, 0, 0, 'a', 'd', 'd', \
  };

  ASSERT_EQ(expectedData, GetChannel().SerializedData) << PrintData(GetChannel().SerializedData) << std::endl << PrintData(expectedData);
  ASSERT_EQ(expectedData.size(), RawSize(response));
}

TEST_F(SubscriptionDeserialization, PublishResponse)
{
  using namespace OpcUa;
  using namespace OpcUa::Binary;

  const std::vector<char> expectedData = {
    1, 0, (char)0x3D, 0x3, // TypeID

    // RequestHeader
    TEST_RESPONSE_HEADER_BINARY_DATA,

    1,0,0,0, // SubscriptionID
    //AvailableSequenceNumbers
    1,0,0,0, // count
    2,0,0,0,
    // MoreNotifications
    1,

    // NotificationData
    1,0,0,0, // SequenceID
    2,0,0,0,0,0,0,0, // PublishTime
    // Data vector
    1,0,0,0, //Count of Data
    0,0, // Data.Header.TypeID
    0,   // Data.Header.Encoding

    // Statuses
    1,0,0,0,
    0,0,0,0,
    // Diagnostics
    2,0,0,0, // Count
    static_cast<DiagnosticInfoMask>(DIM_LOCALIZED_TEXT | DIM_INNER_DIAGNOSTIC_INFO), 4,0,0,0, \
    DIM_ADDITIONAL_INFO, 3, 0, 0, 0, 'a', 'd', 'd', \
  };

  GetChannel().SetData(expectedData);

  PublishResponse response;
  GetStream() >> response;

  ASSERT_EQ(response.TypeID.Encoding, EV_FOUR_BYTE);
  ASSERT_EQ(response.TypeID.FourByteData.NamespaceIndex, 0);
  ASSERT_EQ(response.TypeID.FourByteData.Identifier, OpcUa::PUBLISH_RESPONSE);

  ASSERT_RESPONSE_HEADER_EQ(response.Header);

  ASSERT_EQ(response.Result.Diagnostics.size(), 2);
}
